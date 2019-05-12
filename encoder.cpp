#include "encoder.h"

Encoder::Encoder(QObject *parent) : SRException(parent), mCdcCtx(nullptr), mEnCdc(nullptr), mRawFrame(nullptr)/*mYUVFrame(nullptr)*/, /*mSwsCtx(nullptr),
    mYUVPkt(nullptr)*/mEnedPkt(nullptr), mOutBuf(nullptr), mNextPts(0), mDstNbSamples(0), mMaxDstNbSam(0), mInitStCdcCtx(false) {}

Encoder::~Encoder() {
    mNextPts = 0;
    mInitStCdcCtx = false;
}


//void Encoder::setEnCtx() {
//    mCdcCtx = avcodec_alloc_context3(mEnCdc);
//    if (!mCdcCtx) {
//        Log::instance()->warn("AVCodecContext is null");
//        emit SRException::instance()->error();
//    }
//
//    mCdcCtx->codec_id = AV_CODEC_ID_H264;
//    mCdcCtx->codec_type = AVMEDIA_TYPE_VIDEO;
//    mCdcCtx->pix_fmt = AV_PIX_FMT_YUV420P;
//    mCdcCtx->bit_rate = 400000;
//    //mCdcCtx->gop_size = 12;
//    mCdcCtx->time_base.num = 1;
//    mCdcCtx->time_base.den = 25;
//    //mCdcCtx->qmin = 10;
//    //mCdcCtx->qmax = 51;
//    mCdcCtx->width = ENW;
//    mCdcCtx->height = ENH;
//
//    mCdcCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
//}

//void Encoder::findEncoder() {
//    mEnCdc = avcodec_find_encoder(AV_CODEC_ID_H264);
//    if (!mEnCdc) {
//        Log::instance()->warn("Can't find encoder");
//        emit SRException::instance()->error();
//    }
//}

//void Encoder::initEnCtx() {
//    AVDictionary *param = nullptr;
//    av_dict_set(&param, "preset", "slow", 0);
//    av_dict_set(&param, "tune", "zerolatency", 0);
//
//
//    if (0 > avcodec_open2(mCdcCtx, mEnCdc, &param)) {
//        Log::instance()->warn("Failed in avcodec_open2 func");
//        emit SRException::instance()->error();
//    }
//}

//void Encoder::initConFrame() {
//    mYUVFrame = av_frame_alloc();
//}

int64_t Encoder::getNextPts() {
    return mNextPts;
}

AVRational Encoder::getTB() {
    //if (mCdcCtx) {
    //    return mCdcCtx->time_base;
    //}
    return mCdcCtx->time_base;
}

//void Encoder::init(GenerateFile *mp4File) {
//    findEncoder();
//    setEnCtx();
//    initEnCtx();
//    initConFrame();
//
//    if (!mInitStCdcCtx) {
//        mp4File->initStCdcCtx(mCdcCtx, VD);
//        mInitStCdcCtx = true;
//    }
//}

//void Encoder::encoder(AVFrame *srcFrame, AVCodecContext *deCodecCtx, GenerateFile *mp4File, StT stt) {
//    qDebug() << "Encoder::encoder" << QThread::currentThreadId();
//    if (!mSwsCtx) {
//        mYUVFrame->format = AV_PIX_FMT_YUV420P;
//        mYUVFrame->width = ENW;
//        mYUVFrame->height = ENH;
//        if (0 > av_frame_get_buffer(mYUVFrame, 32)) {
//            Log::instance()->warn("Could not allocate frame data.\n");
//            emit SRException::instance()->error();
//        }
//        mSwsCtx = sws_getContext(deCodecCtx->width, deCodecCtx->height, deCodecCtx->pix_fmt, ENW, ENH,
//                                 AV_PIX_FMT_YUV420P, SWS_BICUBIC, nullptr, nullptr, nullptr);
//        mYUVPkt = av_packet_alloc();
//    }
//
//    //if (!mInitStCdcCtx) {
//    //    mp4File->initStCdcCtx(mCdcCtx, VD);
//    //    mInitStCdcCtx = true;
//    //}
//
//    if (0 > av_frame_make_writable(mYUVFrame)) {
//        Log::instance()->warn("Frame not writable\n");
//        emit SRException::instance()->error();
//    }
//    sws_scale(mSwsCtx, srcFrame->data, srcFrame->linesize, 0, deCodecCtx->height, mYUVFrame->data, mYUVFrame->linesize);
//    mYUVFrame->pts = mNextPts++;
//
//    qDebug() << mYUVFrame->pts;
//    int ret = avcodec_send_frame(mCdcCtx, mYUVFrame);
//    qDebug() << "b";
//    if (0 > ret) {
//        Log::instance()->warn("Error send frame to encoder\n");
//        emit SRException::instance()->error();
//    }
//
//    while(0 <= avcodec_receive_packet(mCdcCtx, mYUVPkt)) {
//        mp4File->generateFile(mYUVPkt, &mCdcCtx->time_base, stt);
//    }
//}
//
//void Encoder::clean() {
//    av_frame_free(&mYUVFrame);
//    av_packet_free(&mYUVPkt);
//    sws_freeContext(mSwsCtx);
//    avcodec_free_context(&mCdcCtx);
//
//    mSwsCtx = nullptr;
//    mInitStCdcCtx = false;
//    mNextPts = 0;
//}
