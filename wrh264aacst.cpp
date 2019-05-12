#include "wrh264aacst.h"
#include "h264encoder.h"
#include "aacencoder.h"
#include "mp4file.h"
#include "publicbuffer.h"

WrH264AacSt::WrH264AacSt(QObject *parent) : WriteSt(parent) {
    mMp4Fi = new Mp4File;
    mH264En = new H264Encoder;
    mAacEn = new AacEncoder;

    connect(mMp4Fi, &Mp4File::errorMp4Fi, this, &WrH264AacSt::errorWrH264AacS);
    connect(mH264En, &H264Encoder::errorH264En, this, &WrH264AacSt::errorWrH264AacS);
    connect(mAacEn, &AacEncoder::errorAACEn, this, &WrH264AacSt::errorWrH264AacS);
}

WrH264AacSt::~WrH264AacSt() {
    if (mAacEn) {
        delete mAacEn;
        mAacEn = nullptr;
    }
    if (mH264En) {
        delete mH264En;
        mH264En = nullptr;
    }
    if (mMp4Fi) {
        delete mMp4Fi;
        mMp4Fi = nullptr;
    }
}

void WrH264AacSt::sendErr() {
    emit errorWrH264AacS();
}

void WrH264AacSt::startingWr(const bool enableVd, const bool enableAu) {
    init(enableVd, enableAu);

    if (!*mInited) {
        mMp4Fi->init(enableVd, enableAu);
        if (enableVd) {
            mH264En->init(mMp4Fi);
        }
        if (enableAu) {
            mAacEn->init(mMp4Fi);
        }

        *mInited = true;
    }

    if (!*mStaingWr) {
        *mStaingWr = true;
        wrVdAuSt();
    }
}

void WrH264AacSt::pausingWr() {
    uninit();
}

void WrH264AacSt::wringFiTra() {
    if (mAlWrTr) {
        if (!*mAlWrTr) {
            mMp4Fi->writeTrailer();
            *mAlWrTr = true;
        }
    }
}

void WrH264AacSt::clean() {
    if (mStoWrCom) {
        delete mStoWrCom;
        mStoWrCom = nullptr;
    }
    if (mAlWrTr) {
        delete mAlWrTr;
        mAlWrTr = nullptr;
    }
    if (mAlWrHe) {
        delete mAlWrHe;
        mAlWrHe = nullptr;
    }
    if (mInited) {
        delete mInited;
        mInited = nullptr;
    }
    if (mStoWr) {
        delete mStoWr;
        mStoWr = nullptr;
    }
    if (mStaingWr) {
        delete mStaingWr;
        mStaingWr = nullptr;
    }
    if (mImStoWrComed) {
        delete mImStoWrComed;
        mImStoWrComed = nullptr;
    }
    if (mImStoWr) {
        delete mImStoWr;
        mImStoWr = nullptr;
    }
    if (mEnAu) {
        delete mEnAu;
        mEnAu = nullptr;
    }
    if (mEnVd) {
        delete mEnVd;
        mEnVd = nullptr;
    }
    if (mH264En) {
        mH264En->clean();
    }
    if (mAacEn) {
        mAacEn->clean();
    }
    if (mMp4Fi) {
        mMp4Fi->clean();
    }
}

void WrH264AacSt::flush() {
    if (*mEnVd) {
        mH264En->encoder(nullptr, mDeVdCtx, mMp4Fi, VD, true);
    }
    if (*mEnAu) {
        mAacEn->encoder(nullptr, mDeAuCtx, mMp4Fi, AU, true);
    }
}

void WrH264AacSt::init(const bool enableVd, const bool enableAu) {
    qDebug() << "WrH264AacSt init thread id: " << QThread::currentThread();
    if (!mEnVd) {
        mEnVd = new bool(enableVd);
    }
    if (!mEnAu) {
        mEnAu = new bool(enableAu);
    }
    if (!mImStoWr) {
        mImStoWr = new bool;
        *mImStoWr = false;
    }
    if (!mImStoWrComed) {
        mImStoWrComed = new bool;
        *mImStoWrComed = false;
    }
    if (!mStaingWr) {
        mStaingWr = new bool;
        *mStaingWr = false;
    }
    if (!mStoWr) {
        mStoWr = new bool;
        *mStoWr = false;
    }
    if (!mInited) {
        mInited = new bool;
        *mInited = false;
    }
    if (!mAlWrHe) {
        mAlWrHe = new bool;
        *mAlWrHe = false;
    }
    if (!mAlWrTr) {
        mAlWrTr = new bool;
        *mAlWrTr = false;
    }
    if (!mStoWrCom) {
        mStoWrCom = new bool;
        *mStoWrCom = false;
    }
}

void WrH264AacSt::uninit() {
    if (mStoWr) {
        *mStoWr = true;
    }
    if (mStaingWr) {
        *mStaingWr = false;
    }
    if (mStoWrCom) {
        *mStoWrCom = false;
    }
}

void WrH264AacSt::wrVdAuSt() {
    qDebug() << "WrH264AacSt wrVdAuSt thread id: " << QThread::currentThread();
    //int VI = 0, AI = 0;
    if (!*mAlWrHe) {
        mMp4Fi->writeHeader();
        *mAlWrHe = true;
    }
    while (true) {
        //if (*mImStoWr) {
        //    *mImStoWrComed = true;
        //    break;
        //}
        if (*mStoWr) {
            //emit sendReVFrsCo(0);
            *mStoWr = false;
            *mStoWrCom = true;
            break;
        }

        try {
            if ((*mEnVd) && ((!*mEnAu) || (0 >= av_compare_ts(mH264En->getNextPts(), mH264En->getTB(),
                                                              mAacEn->getNextPts(), mAacEn->getTB())))) {
                AVFrame *rawVFr = PublicBuffer::instance()->getRawVFr();
                const AVCodecContext *rawVCdcCtx = PublicBuffer::instance()->getRawVCdcCtx();
                if (rawVCdcCtx && rawVFr) {
                    //mH264En->encoder(rawVFr, PublicBuffer::instance()->getRawVCdcCtx(), mMp4Fi, VD);
                    mH264En->encoder(rawVFr, rawVCdcCtx, mMp4Fi, VD);
                }
                if (rawVFr) {
                    av_frame_free(&rawVFr);
                }
                //Log::instance()->info(QString("Write video count: %1").arg(VI++));
            } else {
                while (!mAacEn->getOnAacFrDaCom()) {
                    //if (*mImStoWr) {
                    //    *mImStoWrComed = true;
                    //    break;
                    //}

                    if (*mStoWr) {
                        break;
                    }

                    AVFrame *rawAFr = PublicBuffer::instance()->getRawAFr();
                    const AVCodecContext *rawACdcCtx = PublicBuffer::instance()->getRawACdcCtx();
                    if (rawACdcCtx && rawAFr) {
                        //mAacEn->encoder(rawAFr, PublicBuffer::instance()->getRawACdcCtx(), mMp4Fi, AU);
                        mAacEn->encoder(rawAFr, rawACdcCtx, mMp4Fi, AU);
                    }
                    if (rawAFr) {
                        av_frame_free(&rawAFr);
                    }
                }

                //Log::instance()->info(QString("Write audio count: %1").arg(AI++));
                mAacEn->getOnAacFrDaUnCom();
            }
        } catch (screenrecoder::WrH264AacStException &ex) {
            Log::instance()->warn(ex.what());
            //clean();
            *mImStoWrComed = true;
            emit errorWrH264AacS();
            return;
        }
    }

    //if (*mEnVd) {
    //    mH264En->encoder(nullptr, mDeVdCtx, mMp4Fi, VD, true);
    //}
    //if (*mEnAu) {
    //    mAacEn->encoder(nullptr, mDeAuCtx, mMp4Fi, AU, true);
    //}
}
