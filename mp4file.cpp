#include "mp4file.h"
#include <QMutex>

Mp4File::Mp4File(const QString pcmFiNa, const QString fileName, QObject *parent) : GenerateFile(pcmFiNa, fileName, parent) {

}

Mp4File::~Mp4File() {

}

void Mp4File::openOutDev() {
    avformat_alloc_output_context2(&mFmtCtx, nullptr, nullptr, mOutFile.toLatin1().data());
    if (!mFmtCtx) {
        //Log::instance()->warn("Can't alloc avformat context");
        //sendErr();
        //return;
        throw screenrecoder::Mp4FiException("Can't alloc avformat context");
    }

    mOutFmt = mFmtCtx->oformat;

    if (mOutFmt->flags & AVFMT_GLOBALHEADER) {

    }

}

void Mp4File::initAVIO() {
    if (!(mOutFmt->flags & AVFMT_NOFILE)) {
        mAVIOSu = avio_open(&mFmtCtx->pb, mOutFile.toLatin1().data(), AVIO_FLAG_WRITE);
        if (0 > mAVIOSu) {
            //Log::instance()->warn("Failed in avio_open func");
            //sendErr();
            //return;
            throw screenrecoder::Mp4FiException("Failed in avio_open func");
        }
    }
}

void Mp4File::createVdoSt() {
    mVdoSt = avformat_new_stream(mFmtCtx, nullptr);
    if (!mVdoSt) {
        //Log::instance()->warn("Can't new video stream");
        //sendErr();
        //return;
        throw screenrecoder::Mp4FiException("Can't new video stream");
    }

    mVdoSt->time_base.num = 1;
    mVdoSt->time_base.den = 25;

    mVdoSt->id = mFmtCtx->nb_streams - 1;
}

void Mp4File::createAuSt() {
    mAuSt = avformat_new_stream(mFmtCtx, nullptr);
    if (!mAuSt) {
        //Log::instance()->warn("Can't new video stream");
        //sendErr();
        //return;
        throw screenrecoder::Mp4FiException("Can't new video stream");
    }

    mAuSt->time_base.num = 1;
    mAuSt->time_base.den = 44100;

    mAuSt->id = mFmtCtx->nb_streams - 1;
}

AVStream *Mp4File::createSt() {
    AVStream *st = nullptr;
    st = avformat_new_stream(mFmtCtx, nullptr);
    if (!st) {
        Log::instance()->warn("Can't new video stream");
        return st;
    }

    // 暂时还不太清楚这个索引是必须根据创建流中有几条流来写还是可以自己手动指定索引
    st->id = mFmtCtx->nb_streams - 1;

    return st;
}

void Mp4File::sendErr() {
    emit errorMp4Fi();
}

void Mp4File::init(bool enableVd, bool enableAu) {
    openOutDev();
    if (enableVd) {
        //createVdoSt();
        mVdoSt = createSt();
        if (!mVdoSt) {
            //sendErr();
            //return;
            throw screenrecoder::Mp4FiException("Failed create video stream");
        }

        mVdoSt->time_base.num = 1;
        mVdoSt->time_base.den = 10;
    }
    if (enableAu) {
        //createAuSt();
        mAuSt = createSt();
        if (!mAuSt) {
            //sendErr();
            //return;
          throw screenrecoder::Mp4FiException("Failed create audio stream");
        }

        mAuSt->time_base.num = 1;
        mAuSt->time_base.den = 44100;
    }

    initAVIO();
}

void Mp4File::generateFile(AVPacket *enPkt, const AVRational *timeBase, StT stt) {
    qDebug() << "GenerateFile::generateFile " << QThread::currentThreadId();

    //int ret = -1;

    //if (!mWriteHead) {
    //    av_dump_format(mFmtCtx, 0, mOutFile.toLatin1().data(), 1);

    //    ret = avformat_write_header(mFmtCtx, nullptr);
    //    if (0 > ret) {
    //        Log::instance()->warn("Error write header\n");
    //        sendErr();
    //    }
    //    mWriteHead = true;
    //}

    if (VD == stt) {
        av_packet_rescale_ts(enPkt, *timeBase, mVdoSt->time_base);
        enPkt->stream_index = mVdoSt->id;
        //Log::instance()->info(QString("Video pts: %1").arg(enPkt->pts));
    }

    if (AU == stt) {
        qDebug() << "mCdcCtx tb: " << "num: " << (*timeBase).num << "den: " << (*timeBase).den;
        av_packet_rescale_ts(enPkt, *timeBase, mAuSt->time_base);
        enPkt->stream_index = mAuSt->id;
        //Log::instance()->info(QString("Audio pts: %1").arg(enPkt->pts));
    }

    QMutex mutex;
    mutex.lock();
    int ret = av_interleaved_write_frame(mFmtCtx, enPkt);
    mutex.unlock();
    if (0 > ret) {
        Log::instance()->warn("Write frame failed");
    }

    av_packet_unref(enPkt);
}

void Mp4File::generatePcmFi(AVSampleFormat samFmt, AVFrame *rawFr, uint relSamCo) {
    if (!mOutFi.isOpen()) {
        if (!mOutFi.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            //Log::instance()->warn("Faild open pcm file\n");
            //sendErr();
            //return;
            throw screenrecoder::Mp4FiException("Faild open pcm file\n");
        }
    }

    uint rsc = 0;
    int dataSize = av_get_bytes_per_sample(samFmt);
    if (0 > dataSize) {
        //Log::instance()->warn("Failed to calculate pcm data size\n");
        //sendErr();
        //return;
        throw screenrecoder::Mp4FiException("Failed to calculate pcm data size\n");
    }
    if (av_sample_fmt_is_planar(samFmt)) {
        // 采样格式为平面
        if (-1 == relSamCo) {
            rsc = rawFr->nb_samples;
        } else {
            rsc = relSamCo;
        }
        for (int i = 0; i < rsc; i++) {
            for (int ch = 0; ch < rawFr->channels; ch++) {
                mOutFi.write(reinterpret_cast<char*>(rawFr->data[ch] + dataSize * i), dataSize);
            }
        }
    } else {
        mOutFi.write(reinterpret_cast<char*>(rawFr->data[0]), rawFr->linesize[0]);
    }
}

void Mp4File::clean() {
    if (0 <= mAVIOSu) {
        avio_closep(&mFmtCtx->pb);
    }
    avformat_free_context(mFmtCtx);
    if (mOutFi.isOpen()) {
        mOutFi.close();
    }
    mFmtCtx = nullptr;

    mWriteHead = false;
    mAVIOSu = -1;
}

void Mp4File::writeHeader() {
    if (!mWriteHead) {
        av_dump_format(mFmtCtx, 0, mOutFile.toLatin1().data(), 1);

        int ret = avformat_write_header(mFmtCtx, nullptr);
        if (0 > ret) {
            //Log::instance()->warn("Error write header\n");
            //sendErr();
            //return;
            throw screenrecoder::Mp4FiException("Error write header\n");
        }
        mWriteHead = true;
    }
}

void Mp4File::writeTrailer() {
    if (mWriteHead) {
        av_write_trailer(mFmtCtx);
    }
}

void Mp4File::initStCdcCtx(AVCodecContext *enCdcCtx, StT stT) {
    switch (stT) {
    case VD:
        avcodec_parameters_from_context(mVdoSt->codecpar, enCdcCtx);
        break;
    case AU:
        avcodec_parameters_from_context(mAuSt->codecpar, enCdcCtx);
        break;
    default:
        break;
    }
}
