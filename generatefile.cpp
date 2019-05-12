#include "generatefile.h"

GenerateFile::GenerateFile(const QString pcmFiNa, const QString fileName, QObject *parent) : SRException(parent), mFmtCtx(nullptr), mOutFmt(nullptr), mVdoSt(nullptr), mAuSt(nullptr),
    mOutFile(fileName), mWriteHead(false), mOutFi(pcmFiNa) {}
    //mOutFile(QCoreApplication::applicationDirPath() + QString("/ScrrenRecoder.mp4")), mWriteHead(false) {}

GenerateFile::~GenerateFile() {
    mWriteHead = false;
    if (mOutFi.isOpen()) {
        mOutFi.close();
    }
}

//void GenerateFile::openOutDev() {
//    avformat_alloc_output_context2(&mFmtCtx, nullptr, nullptr, mOutFile.toLatin1().data());
//    if (!mFmtCtx) {
//        Log::instance()->warn("Can't alloc avformat context");
//        emit SRException::instance()->error();
//    }
//
//    mOutFmt = mFmtCtx->oformat;
//
//    if (mOutFmt->flags & AVFMT_GLOBALHEADER) {
//
//    }
//
//}
//
//void GenerateFile::initAVIO() {
//    if (!(mOutFmt->flags & AVFMT_NOFILE)) {
//        mAVIOSu = avio_open(&mFmtCtx->pb, mOutFile.toLatin1().data(), AVIO_FLAG_WRITE);
//        if (0 > mAVIOSu) {
//            Log::instance()->warn("Failed in avio_open func");
//            emit SRException::instance()->error();
//        }
//    }
//}
//
//void GenerateFile::createVdoSt() {
//    mVdoSt = avformat_new_stream(mFmtCtx, nullptr);
//    if (!mVdoSt) {
//        Log::instance()->warn("Can't new video stream");
//        emit SRException::instance()->error();
//    }
//
//    mVdoSt->time_base.num = 1;
//    mVdoSt->time_base.den = 25;
//
//    mVdoSt->id = mFmtCtx->nb_streams - 1;
//}
//
//void GenerateFile::createAuSt() {
//    mAuSt = avformat_new_stream(mFmtCtx, nullptr);
//    if (!mAuSt) {
//        Log::instance()->warn("Can't new video stream");
//        emit SRException::instance()->error();
//    }
//
//    mAuSt->time_base.num = 1;
//    mAuSt->time_base.den = 44100;
//
//    mAuSt->id = mFmtCtx->nb_streams - 1;
//}
//
//AVStream *GenerateFile::createSt() {
//    AVStream *st = nullptr;
//    st = avformat_new_stream(mFmtCtx, nullptr);
//    if (!st) {
//        Log::instance()->warn("Can't new video stream");
//        return st;
//    }
//
//    // 暂时还不太清楚这个索引是必须根据创建流中有几条流来写还是可以自己手动指定索引
//    st->id = mFmtCtx->nb_streams - 1;
//
//    return st;
//}
//
//void GenerateFile::init(bool enableVd, bool enableAu) {
//    openOutDev();
//    if (enableVd) {
//        //createVdoSt();
//        mVdoSt = createSt();
//        if (!mVdoSt) {
//            emit SRException::instance()->error();
//        }
//
//        mVdoSt->time_base.num = 1;
//        mVdoSt->time_base.den = 25;
//    }
//    if (enableAu) {
//        //createAuSt();
//        mAuSt = createSt();
//        if (!mAuSt) {
//            emit SRException::instance()->error();
//        }
//
//        mAuSt->time_base.num = 1;
//        mAuSt->time_base.den = 44100;
//    }
//
//    initAVIO();
//
//    av_dump_format(mFmtCtx, 0, mOutFile.toLatin1().data(), 1);
//}
//
//void GenerateFile::generateFile(AVPacket *enPkt, const AVRational *timeBase, StT stt) {
//    qDebug() << "GenerateFile::generateFile " << QThread::currentThreadId();
//
//    int ret = -1;
//
//    if (!mWriteHead) {
//        ret = avformat_write_header(mFmtCtx, nullptr);
//        if (0 > ret) {
//            Log::instance()->warn("Error write header\n");
//            emit SRException::instance()->error();
//        }
//        mWriteHead = true;
//    }
//
//    if (VD == stt) {
//        av_packet_rescale_ts(enPkt, *timeBase, mVdoSt->time_base);
//        enPkt->stream_index = mVdoSt->id;
//    }
//
//    if (AU == stt) {
//        av_packet_rescale_ts(enPkt, *timeBase, mAuSt->time_base);
//        enPkt->stream_index = mAuSt->id;
//    }
//
//    ret = av_interleaved_write_frame(mFmtCtx, enPkt);
//    if (0 > ret) {
//        Log::instance()->warn("Write frame failed");
//    }
//
//    av_packet_unref(enPkt);
//}
//
//void GenerateFile::clean() {
//    //av_write_trailer(mFmtCtx);
//
//    if (0 <= mAVIOSu) {
//        avio_closep(&mFmtCtx->pb);
//    }
//    avformat_free_context(mFmtCtx);
//
//    mWriteHead = false;
//}
//
//void GenerateFile::writeTrailer() {
//    av_write_trailer(mFmtCtx);
//}
//
//void GenerateFile::initStCdcCtx(AVCodecContext *enCdcCtx, StT stT) {
//    switch (stT) {
//    case VD:
//        avcodec_parameters_from_context(mVdoSt->codecpar, enCdcCtx);
//        break;
//    case AU:
//        avcodec_parameters_from_context(mAuSt->codecpar, enCdcCtx);
//        break;
//    default:
//        break;
//    }
//}
//
