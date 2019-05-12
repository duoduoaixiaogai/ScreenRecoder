#include "readaudiost.h"
#include "audiodecoder.h"
#include "publicbuffer.h"

ReadAudioSt::ReadAudioSt(QVector<QPair<QString, QString> > fields, const uint fps, QObject *parent) : ReadSt(fps, parent) {
    //ffmpeg -y -f alsa -ac 2 -ar 44100 -i hw:Loopback,1,0 /tmp/alsaout.wav
    //mReS = new AudioDecoder(fields, "alsa", "hw:Loopback,1,0");
    mReS = new AudioDecoder(fields);
    //connect(mReS, &AudioDecoder::errorRawDe, this, &ReadAudioSt::errorReAuS);
}

ReadAudioSt::~ReadAudioSt() {
    if (mReS) {
        delete mReS;
        mReS = nullptr;
    }
}

void ReadAudioSt::sendErr() {
    emit errorReAuS();
}

void ReadAudioSt::startingRe() {
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
                mDeCdcCtx = mReS->decoder(mRawFrs, AU);
                PublicBuffer::instance()->setRawACdcCtx(mDeCdcCtx);
                if (!mRawFrs->isEmpty()) {
                    PublicBuffer::instance()->putRawAFr(mRawFrs->dequeue());
                }
            } catch (screenrecoder::RawDeException &ex) {
                Log::instance()->warn(ex.what());
                //clean();
                *mImStoReComed = true;
                emit errorReAuS();
                return;
            }
        }
    }
}

void ReadAudioSt::clean() {
    ReadSt::clean();
    mReS->clean();
}
