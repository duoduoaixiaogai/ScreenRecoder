#include "readst.h"
#include "decoder.h"

ReadSt::ReadSt(const uint fps, QObject *parent) : SRException(parent), mRawFrs(nullptr), mDeCdcCtx(nullptr), mReS(nullptr),
    mStoRe(nullptr), mStaingRe(nullptr), mInited(nullptr), mPaused(nullptr), mImStoRe(nullptr),
    mImStoReComed(nullptr), mStoReCom(nullptr), mFps(fps) {

}

ReadSt::~ReadSt() {

}

void ReadSt::init() {
    if (!mRawFrs) {
        mRawFrs = new QQueue<AVFrame*>;
    }

    if (!mImStoRe) {
        mImStoRe = new bool;
        *mImStoRe = false;
    }

    if (!mImStoReComed) {
        mImStoReComed = new bool;
        *mImStoReComed = false;
    }

    if (!mStoRe) {
        mStoRe = new bool;
        *mStoRe = false;
    }

    if (!mStaingRe) {
        mStaingRe = new bool;
        *mStaingRe = false;
    }

    if (!mInited) {
        mInited = new bool;
        *mInited = false;
    }

    if (!mPaused) {
        mPaused = new bool;
        *mPaused = false;
    }

    if (!mStoReCom) {
        mStoReCom = new bool;
        *mStoReCom = false;
    }
}

void ReadSt::unInit() {
    if (mStaingRe) {
        *mStaingRe = false;
    }
    if (mStoRe) {
        *mStoRe = true;
    }
    if (mPaused) {
        *mPaused = true;
    }
    //if (mReS) {
    //    mReS->closeInDev();
    //    if (mPaused) {
    //        *mPaused = true;
    //    }
    //}
    if (mStoReCom) {
        *mStoReCom = false;
    }
}

void ReadSt::startingRe() {}

void ReadSt::pausingRe() {
    unInit();
}

void ReadSt::imStoRe() {
    if (mImStoRe) {
        *mImStoRe = true;
    }
}

bool ReadSt::imStoReComed() {
    if (mImStoReComed) {
        return *mImStoReComed;
    }
    return true;
}

bool ReadSt::alStaRe() {
    if (mStaingRe) {
        return *mStaingRe;
    }
    return false;
}

bool ReadSt::stoReCom() {
    if (mStoReCom) {
        return *mStoReCom;
    }
    return true;
}

void ReadSt::clean() {
    if (mStoReCom) {
        delete mStoReCom;
        mStoReCom = nullptr;
    }

    if (mPaused) {
        delete mPaused;
        mPaused = nullptr;
    }

    if (mInited) {
        delete mInited;
        mInited = nullptr;
    }

    if (mStaingRe) {
        delete mStaingRe;
        mStaingRe = nullptr;
    }

    if (mStoRe) {
        delete mStoRe;
        mStoRe = nullptr;
    }

    if (mImStoReComed) {
        delete mImStoReComed;
        mImStoReComed = nullptr;
    }

    if (mImStoRe) {
        delete mImStoRe;
        mImStoRe = nullptr;
    }

    if (mRawFrs) {
        delete mRawFrs;
        mRawFrs = nullptr;
    }
}
