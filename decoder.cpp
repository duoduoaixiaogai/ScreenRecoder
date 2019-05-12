#include "decoder.h"

Decoder::Decoder(QVector<QPair<QString, QString> > fields, const QString format, const QString devName, AVMediaType meT, QObject *parent) :
    RawDeException(parent), mFmtCtx(nullptr), mInFmt(nullptr), mCdcCtx(nullptr),
    mDeCdc(nullptr), mSrcFrame(nullptr), mSrcPkt(nullptr), mCdcPars(nullptr),
    mFmt(format), mDevName(devName), mFields(fields),/*mVdoIdxSt(0), */mIdxSt(0), mMeT(meT) {}

Decoder::~Decoder() {}

void Decoder::allocFmtCtx() {
    mFmtCtx = avformat_alloc_context();
    if (!mFmtCtx) {
        //Log::instance()->warn("Can't alloc avformat context");
        //sendErr();
        //return;
        throw screenrecoder::RawDeException("Can't alloc avformat context");
    }
}

void Decoder::openInDev(QVector<QPair<QString, QString> > fields) {
    AVDictionary *dct = nullptr;
    if (!fields.isEmpty()) {
        for (int i = 0; i < fields.count(); i++) {
            av_dict_set(&dct, fields.at(i).first.toLatin1().data(),
                        fields.at(i).second.toLatin1().data(), 0);
        }
    }

    //mFmtCtx = avformat_alloc_context();
    //if (!mFmtCtx) {
    //    Log::instance()->warn("Can't alloc avformat context");
    //    sendErr();
    //}

    mInFmt = av_find_input_format(mFmt.toLatin1().data());
    if (!mInFmt) {
        //Log::instance()->warn("Find input format failed");
        //sendErr();
        //return;
        throw screenrecoder::RawDeException("Find input format failed");
    }

    if (0 > avformat_open_input(&mFmtCtx, mDevName.toLatin1().data(), mInFmt, &dct)) {
        //Log::instance()->warn("Failed in avformat_open_input func");
        //sendErr();
        //return;
        throw screenrecoder::RawDeException("Failed in avformat_open_input func");
    }
}

void Decoder::openInDev() {
    openInDev(mFields);
}

void Decoder::closeInDev() {
    avformat_close_input(&mFmtCtx);
}

//void Decoder::findVideoSt() {
//    for (unsigned int i = 0; i < mFmtCtx->nb_streams; i++) {
//        if (AVMEDIA_TYPE_VIDEO == mFmtCtx->streams[i]->codecpar->codec_type) {
//            mVdoIdxSt = i;
//            break;
//        }
//    }
//}

void Decoder::findSt() {
    for (unsigned int i = 0; i < mFmtCtx->nb_streams; i++) {
        if (mMeT == mFmtCtx->streams[i]->codecpar->codec_type) {
            mIdxSt = i;
            break;
        }
    }
}

//void Decoder::findVideoDe() {
//    mCdcPars = mFmtCtx->streams[mVdoIdxSt]->codecpar;
//    if (!mCdcPars) {
//        Log::instance()->warn("Can't find codecpar in avformatcontext's stream");
//        return;
//    }
//
//    mDeCdc = avcodec_find_decoder(mCdcPars->codec_id);
//    if (!mDeCdc) {
//        Log::instance()->warn("Can't find decoder");
//        return;
//    }
//}

void Decoder::findDe() {
    mCdcPars = mFmtCtx->streams[mIdxSt]->codecpar;
    if (!mCdcPars) {
        //Log::instance()->warn("Can't find codecpar in avformatcontext's stream");
        //sendErr();
        //return;
        throw screenrecoder::RawDeException("Can't find codecpar in avformatcontext's stream");
    }

    mDeCdc = avcodec_find_decoder(mCdcPars->codec_id);
    if (!mDeCdc) {
        //Log::instance()->warn("Can't find decoder");
        //sendErr();
        //return;
        throw screenrecoder::RawDeException("Can't find decoder");
    }
}

void Decoder::initDeCtx() {
    mCdcCtx = avcodec_alloc_context3(mDeCdc);

    if (0 > avcodec_parameters_to_context(mCdcCtx, mCdcPars)) {
        //Log::instance()->warn("Could not copy the stream parameters");
        //sendErr();
        //return;
        throw screenrecoder::RawDeException("Could not copy the stream parameters");
    }

    int ret = avcodec_open2(mCdcCtx, mDeCdc, nullptr);
    if (0 > ret) {
        //Log::instance()->warn("Can't init decoder context. error code");
        //sendErr();
        //return;
        throw screenrecoder::RawDeException("Can't init decoder context. error code");
    }
}

void Decoder::allocPktFrame() {
    mSrcPkt = av_packet_alloc();
    mSrcFrame = av_frame_alloc();
}

void Decoder::init() {
    allocFmtCtx();
    openInDev(mFields);
    findSt();
    //findVideoSt();
    findDe();
    //findVideoDe();
    initDeCtx();
    allocPktFrame();
}

//void Decoder::decoder(Encoder *h264En, GenerateFile *mp4File) {
//    qDebug() << "Decoder::decoder" << QThread::currentThreadId();
//    if (0 > av_read_frame(mFmtCtx, mSrcPkt)) {
//        Log::instance()->info("Read an frame failed");
//    }
//    if (mVdoIdxSt == static_cast<unsigned int>(mSrcPkt->stream_index)) {
//        if (0 > avcodec_send_packet(mCdcCtx, mSrcPkt)) {
//            Log::instance()->warn("Error sending a packet for decoding");
//            return;
//        }
//
//        while (0 <= avcodec_receive_frame(mCdcCtx, mSrcFrame)) {
//            h264En->encoder(mSrcFrame, mCdcCtx, mp4File);
//        }
//
//        av_packet_unref(mSrcPkt);
//    }
//}

void Decoder::decoder(Encoder *en, GenerateFile *file, StT stt) {
    qDebug() << "Decoder::decoder" << QThread::currentThreadId();
    if (0 > av_read_frame(mFmtCtx, mSrcPkt)) {
        Log::instance()->info("Read an frame failed");
    }
    if (mIdxSt == static_cast<unsigned int>(mSrcPkt->stream_index)) {
        if (0 > avcodec_send_packet(mCdcCtx, mSrcPkt)) {
            //Log::instance()->warn("Error sending a packet for decoding");
            //sendErr();
            //return;
            throw screenrecoder::RawDeException("Error sending a packet for decoding");
        }

        while (0 <= avcodec_receive_frame(mCdcCtx, mSrcFrame)) {
            qDebug() << "src nb_samples" << mSrcFrame->nb_samples;
            en->encoder(mSrcFrame, mCdcCtx, file, stt);
            //file->generatePcmFi(mCdcCtx->sample_fmt, mSrcFrame, -1);
        }

        av_packet_unref(mSrcPkt);
    }
}

const AVCodecContext *Decoder::decoder(QQueue<AVFrame *> *rawFrs, StT stt) {
    qDebug() << "Decoder::decoder" << QThread::currentThreadId();
    if (0 > av_read_frame(mFmtCtx, mSrcPkt)) {
        Log::instance()->info("Read an frame failed");
    }
    if (mIdxSt == static_cast<unsigned int>(mSrcPkt->stream_index)) {
        if (0 > avcodec_send_packet(mCdcCtx, mSrcPkt)) {
            //Log::instance()->warn("Error sending a packet for decoding");
            //sendErr();
            //return nullptr;
            throw screenrecoder::RawDeException("Error sending a packet for decoding");
        }

        while (0 <= avcodec_receive_frame(mCdcCtx, mSrcFrame)) {
            //qDebug() << "src nb_samples" << mSrcFrame->nb_samples;
            AVFrame *tmpFr = av_frame_alloc();
            if (VD == stt) {
                //int second = mSrcFrame->pts * av_q2d(mFmtCtx->streams[mIdxSt]->time_base);
                //Log::instance()->info(QString("Get video frame: %1").arg(vI++));
                //Log::instance()->info(QString("video frame time: %1").arg(second));
                tmpFr->format = mSrcFrame->format;
                tmpFr->width = mSrcFrame->width;
                tmpFr->height = mSrcFrame->height;
            } else {
                //int second = mSrcFrame->pts * av_q2d(mFmtCtx->streams[mIdxSt]->time_base);
                //Log::instance()->info(QString("Get audio frame: %1").arg(aI++));
                //Log::instance()->info(QString("audio frame time: %1").arg(second));
                tmpFr->format = mSrcFrame->format;
                tmpFr->nb_samples = mSrcFrame->nb_samples;
                tmpFr->channel_layout = mSrcFrame->channel_layout;
                tmpFr->channels = mSrcFrame->channels;
            }
            av_frame_get_buffer(tmpFr, 0);
            if (0 <= av_frame_copy(tmpFr, mSrcFrame)) {
                av_frame_copy_props(tmpFr, mSrcFrame);
                rawFrs->enqueue(tmpFr);
            }
        }

        av_packet_unref(mSrcPkt);
    }

    return mCdcCtx;
}

void Decoder::clean() {
    av_frame_free(&mSrcFrame);
    av_packet_free(&mSrcPkt);
    avcodec_free_context(&mCdcCtx);
    avformat_close_input(&mFmtCtx);
    avformat_free_context(mFmtCtx);

    mSrcFrame = nullptr;
    mSrcPkt = nullptr;
    mCdcCtx = nullptr;
    mFmtCtx = nullptr;
}
