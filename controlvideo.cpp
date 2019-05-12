// This file deprecated
#include "controlvideo.h"


ControlVideo::ControlVideo(QObject *parent) : CtlVdException(parent), mH264En(nullptr), mRawDe(nullptr), mStaRe(nullptr) {
    mRawDe = new VideoDecoder;
    mH264En = new H264Encoder;

    connect(mRawDe, &VideoDecoder::errorRawDe, this, &ControlVideo::errorVd);
    connect(dynamic_cast<H264Encoder*>(mH264En), &H264Encoder::errorH264En, this, &ControlVideo::errorVd);
    connect(this, &ControlVideo::errorCtlVd, this, &ControlVideo::errorVd);
}

ControlVideo::~ControlVideo() {
    if (mH264En) {
        delete mH264En;
        mH264En = nullptr;
    }
    if (mRawDe) {
        delete mRawDe;
        mRawDe = nullptr;
    }
    if (mStaRe) {
        delete mStaRe;
        mStaRe = nullptr;
    }
}


//void ControlVideo::init(bool enableVd, bool enableAu) {
void ControlVideo::init(GenerateFile *file) {
    qDebug() << "ControlVideo::init " << QThread::currentThreadId();
    if (!file) {
        Log::instance()->warn("Error generate file object is null");
        sendErr();
    }
    mMp4File = file;
    mRawDe->init();
    mH264En->init(file);

    mStaRe = new bool;
    *mStaRe = false;
}

void ControlVideo::ctlVideo() {
    //while(true) {
    //   if (*mStaRe) {
    //        mRawDe->decoder(mH264En, mMp4File);
    //   }

    //   QTest::qWait(10);
    //}
}

void ControlVideo::clean() {
    mH264En->clean();
    mRawDe->clean();
}

void ControlVideo::staReVd() {
    *mStaRe = true;
}

void ControlVideo::stoReVd() {
    *mStaRe = false;
}

void ControlVideo::writeVd(StT stt) {
    mRawDe->decoder(mH264En, mMp4File, stt);
}

int64_t ControlVideo::getNextPts() {
    return mH264En->getNextPts();
}

AVRational ControlVideo::getTB() {
    return mH264En->getTB();
}
