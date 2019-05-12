// This file deprecated
#include "controlaudio.h"


ControlAudio::ControlAudio(QObject *parent) : CtlAuException(parent), mMp4File(nullptr), mAuDe(nullptr), mAACEn(nullptr), mStaRe(nullptr) {
    mAuDe = new AudioDecoder;
    mAACEn = new AacEncoder;

    connect(mAuDe, &AudioDecoder::errorRawDe, this, &ControlAudio::errorAu);
    connect(mAACEn, &AacEncoder::errorAACEn, this, &ControlAudio::errorAu);
    connect(this, &ControlAudio::errorCtlAu, this, &ControlAudio::errorAu);
}

ControlAudio::~ControlAudio() {
    if (!mAuDe) {
        delete mAuDe;
        mAuDe = nullptr;
    }
    if (mAACEn) {
        delete mAACEn;
        mAACEn = nullptr;
    }
    if (mStaRe) {
        delete mStaRe;
        mStaRe = nullptr;
    }
}

void ControlAudio::init(GenerateFile *file) {
    if (!file) {
        Log::instance()->warn("Error generate file object is null");
        sendErr();
    }
    mMp4File = file;
    mAuDe->init();
    mAACEn->init(file);

    mStaRe = new bool;
    *mStaRe = false;
}

void ControlAudio::ctlAudio() {
    //while (true) {
    //    if (*mStaRe) {
    //        mAuDe->decoder(mAACEn, mMp4File);
    //    }

    //    QTest::qWait(10);
    //}
}

void ControlAudio::clean() {
    mAACEn->clean();
    mAuDe->clean();
}

void ControlAudio::staReAu() {
    *mStaRe = true;
}

void ControlAudio::stoReAu() {
    *mStaRe = false;
}

void ControlAudio::writeAu(StT stt) {
    while (!mAACEn->getOnAacFrDaCom()) {
      mAuDe->decoder(mAACEn, mMp4File, stt);
    }

    mAACEn->getOnAacFrDaUnCom();
}

int64_t ControlAudio::getNextPts() {
    return mAACEn->getNextPts();
}

AVRational ControlAudio::getTB() {
    return mAACEn->getTB();
}
