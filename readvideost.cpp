#include "readvideost.h"
#include "videodecoder.h"
#include "publicbuffer.h"

ReadVideoSt::ReadVideoSt(QVector<QPair<QString, QString> > fields, const uint fps, QObject *parent) : ReadSt(fps, parent) {
    //mReS = new VideoDecoder(fields, "x11grab", ":0.0");
    mReS = new VideoDecoder(fields, "gdigrab", "desktop");
    //mReS = new VideoDecoder(fields);
    //connect(mReS, &VideoDecoder::errorRawDe, this, &ReadVideoSt::errorReVdS);
}

ReadVideoSt::~ReadVideoSt() {
    if (mReS) {
        delete mReS;
        mReS = nullptr;
    }
}

void ReadVideoSt::sendErr() {
    emit errorReVdS();
}

void ReadVideoSt::startingRe() {
    init();

    if (!*mInited) {
        mReS->init();
        *mInited = true;
    }

    if (*mPaused) {
        mReS->openInDev();
        *mPaused = false;
    }

    if (!*mStaingRe) {
        *mStaingRe = true;

        while (true) {
            //if (*mImStoRe) {
            //    *mImStoReComed = true;
            //    break;
            //}
            if (*mStoRe) {
                mReS->closeInDev();
                *mStaingRe = false;
                *mStoRe = false;
                *mStoReCom = true;
                break;
            }

            try {
                mDeCdcCtx = mReS->decoder(mRawFrs, VD);
                PublicBuffer::instance()->setRawVCdcCtx(mDeCdcCtx);
                if (!mRawFrs->isEmpty()) {
                    PublicBuffer::instance()->putRawVFr(mRawFrs->dequeue());
                }
            } catch (screenrecoder::RawDeException &ex) {
                Log::instance()->warn(ex.what());
                //clean();
                *mImStoReComed = true;
                emit errorReVdS();
                return;
            }
        }
    }
}

void ReadVideoSt::clean() {
    ReadSt::clean();
    mReS->clean();
}
