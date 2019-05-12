#include "h264encoder.h"

H264Encoder::H264Encoder(QObject *parent) : Encoder(parent), mSwsCtx(nullptr) {}

H264Encoder::~H264Encoder() {
    mNextPts = 0;
    mInitStCdcCtx = false;
}

void H264Encoder::setEnCtx() {
    mCdcCtx = avcodec_alloc_context3(mEnCdc);
    if (!mCdcCtx) {
        //Log::instance()->warn("AVCodecContext is null");
        //sendErr();
        //return;
        throw screenrecoder::H264EnException("AVCodecContext is null");
    }

    mCdcCtx->codec_id = AV_CODEC_ID_H264;
    mCdcCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    mCdcCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    mCdcCtx->bit_rate = 400000;
    //mCdcCtx->gop_size = 12;
    mCdcCtx->time_base.num = 1;
    mCdcCtx->time_base.den = 10;
    //mCdcCtx->qmin = 10;
    //mCdcCtx->qmax = 51;
    mCdcCtx->width = ENW;
    mCdcCtx->height = ENH;

    mCdcCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
}

void H264Encoder::findEncoder() {
    mEnCdc = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!mEnCdc) {
        //Log::instance()->warn("Can't find encoder");
        //sendErr();
        //return;
        throw screenrecoder::H264EnException("Can't find encoder");
    }
}

void H264Encoder::initEnCtx() {
    AVDictionary *param = nullptr;
    av_dict_set(&param, "preset", "slow", 0);
    av_dict_set(&param, "tune", "zerolatency", 0);


    if (0 > avcodec_open2(mCdcCtx, mEnCdc, &param)) {
        //Log::instance()->warn("Failed in avcodec_open2 func");
        //sendErr();
        //return;
        throw screenrecoder::H264EnException("Failed in avcodec_open2 func");
    }
}

void H264Encoder::initConFrame() {
    mRawFrame = av_frame_alloc();
}

int64_t H264Encoder::getNextPts() {
    return mNextPts;
}

AVRational H264Encoder::getTB() {
    //if (mCdcCtx) {
    //    return mCdcCtx->time_base;
    //}
    return mCdcCtx->time_base;
}

void H264Encoder::sendErr() {
    emit errorH264En();
}

void H264Encoder::init(GenerateFile *mp4File) {
    findEncoder();
    setEnCtx();
    initEnCtx();
    initConFrame();

    if (!mInitStCdcCtx) {
        mp4File->initStCdcCtx(mCdcCtx, VD);
        mInitStCdcCtx = true;
    }
}

void H264Encoder::encoder(AVFrame *srcFrame, const AVCodecContext *deCodecCtx, GenerateFile *mp4File, StT stt, bool fluFr) {
    qDebug() << "Encoder::encoder" << QThread::currentThreadId();
    if (!mSwsCtx) {
        mRawFrame->format = AV_PIX_FMT_YUV420P;
        mRawFrame->width = ENW;
        mRawFrame->height = ENH;
        if (0 > av_frame_get_buffer(mRawFrame, 32)) {
            //Log::instance()->warn("Could not allocate frame data.\n");
            //sendErr();
            //return;
            throw screenrecoder::H264EnException("Could not allocate frame data.\n");
        }
        mSwsCtx = sws_getContext(deCodecCtx->width, deCodecCtx->height, deCodecCtx->pix_fmt, ENW, ENH,
                                 AV_PIX_FMT_YUV420P, SWS_BICUBIC, nullptr, nullptr, nullptr);
        mEnedPkt = av_packet_alloc();
    }

    //if (!mInitStCdcCtx) {
    //    mp4File->initStCdcCtx(mCdcCtx, VD);
    //    mInitStCdcCtx = true;
    //}

    AVFrame *rawFr = nullptr;

    if (!fluFr) {
        if (0 > av_frame_make_writable(mRawFrame)) {
            //Log::instance()->warn("Frame not writable\n");
            //sendErr();
            //return;
            throw screenrecoder::H264EnException("Frame not writable\n");
        }

        if ((ENW != deCodecCtx->width) || (ENH != deCodecCtx->height) || (AV_PIX_FMT_YUV420P != deCodecCtx->pix_fmt)) {
            sws_scale(mSwsCtx, srcFrame->data, srcFrame->linesize, 0, deCodecCtx->height, mRawFrame->data, mRawFrame->linesize);
            rawFr = mRawFrame;
        } else {
            rawFr = srcFrame;
        }
        rawFr->pts = mNextPts++;
    }
    //mRawFrame->pts = mNextPts++;

    //qDebug() << mRawFrame->pts;
    int ret = avcodec_send_frame(mCdcCtx, rawFr);
    //int ret = avcodec_send_frame(mCdcCtx, mRawFrame);
    if (0 > ret) {
        //Log::instance()->warn("Error send frame to encoder\n");
        //sendErr();
        //return;
        throw screenrecoder::H264EnException(QString("Error send video frame to encoder! "
                                                     "Error code: %1; Error string: %2\n").arg(ret).
                                                     arg(PublicBuffer::instance()->getErrStr(ret)));
    }

    while(0 <= avcodec_receive_packet(mCdcCtx, mEnedPkt)) {
        mp4File->generateFile(mEnedPkt, &mCdcCtx->time_base, stt);
    }
}

void H264Encoder::clean() {
    av_frame_free(&mRawFrame);
    av_packet_free(&mEnedPkt);
    sws_freeContext(mSwsCtx);
    avcodec_free_context(&mCdcCtx);

    mRawFrame = nullptr;
    mEnedPkt = nullptr;
    mSwsCtx = nullptr;
    mCdcCtx = nullptr;
    mInitStCdcCtx = false;
    mNextPts = 0;
}
