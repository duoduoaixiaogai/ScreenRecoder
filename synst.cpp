#include "synst.h"
#include "mp4file.h"
#include "h264encoder.h"
#include "aacencoder.h"

SynSt::SynSt(QObject *parent) : SRException(parent), mRawVFrs(nullptr), mRawAFrs(nullptr),
    mStoSyn(nullptr), mStaingSyn(nullptr), mInited(nullptr), mAlWrHe(nullptr), mAlWrTr(nullptr),
    mImStoSyn(nullptr), mImStoSynComed(nullptr), mStoSynCom(nullptr), mEnVd(nullptr), mEnAu(nullptr),
    mVdCdcCtx(nullptr), mAuCdcCtx(nullptr) {
    mMp4Fi = new Mp4File;
    mH264En = new H264Encoder;
    mAacEn = new AacEncoder;

    connect(mMp4Fi, &Mp4File::errorMp4Fi, this, &SynSt::errorSynS);
    connect(mH264En, &H264Encoder::errorH264En, this, &SynSt::errorSynS);
    connect(mAacEn, &AacEncoder::errorAACEn, this, &SynSt::errorSynS);
}

SynSt::~SynSt() {
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

bool SynSt::alStaSyn() {
    if(mStaingSyn) {
        return *mStaingSyn;
    }
    return false;
}

bool SynSt::stoSynCom() {
    if (mStoSynCom) {
        return *mStoSynCom;
    }
    return false;
}

bool SynSt::imStoSynComed() {
    if (mImStoSynComed) {
        return *mImStoSynComed;
    }
    return false;
}

void SynSt::startingSyn(const bool enableVd, const bool enableAu) {
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

    if (!*mStaingSyn) {
        *mStaingSyn = true;
        synVdAuSt();
    }
}

void SynSt::pausingSyn() {
    unInit();
}

void SynSt::imStoSyn() {
    if (mImStoSyn) {
        *mImStoSyn = true;
    }
}

void SynSt::clean() {
    if (!mRawVFrs->isEmpty()) {
        for (int i = 0; i < mRawVFrs->count(); i++) {
            AVFrame *rawFr = mRawVFrs->dequeue();
            av_frame_free(&rawFr);
        }
    }

    if (!mRawAFrs->isEmpty()) {
        for (int i = 0; i < mRawAFrs->count(); i++) {
            AVFrame *rawFr = mRawAFrs->dequeue();
            av_frame_free(&rawFr);
        }
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
    if (mRawVFrs) {
        delete mRawVFrs;
        mRawVFrs = nullptr;
    }
    if (mRawAFrs) {
        delete mRawAFrs;
        mRawAFrs = nullptr;
    }
    if (mImStoSynComed) {
        delete mImStoSynComed;
        mImStoSynComed = nullptr;
    }
    if (mImStoSyn) {
        delete mImStoSyn;
        mImStoSyn = nullptr;
    }
    if (mStoSynCom) {
        delete mStoSynCom;
        mStoSynCom = nullptr;
    }
    if (mStoSyn) {
        delete mStoSyn;
        mStoSyn = nullptr;
    }
    if (mStaingSyn) {
        delete mStaingSyn;
        mStaingSyn = nullptr;
    }
    if (mInited) {
        delete mInited;
        mInited = nullptr;
    }
    if (mAlWrTr) {
        delete mAlWrTr;
        mAlWrTr = nullptr;
    }
    if (mAlWrHe) {
        delete mAlWrHe;
        mAlWrHe = nullptr;
    }
    if (mEnAu) {
        delete mEnAu;
        mEnAu = nullptr;
    }
    if (mEnVd) {
        delete mEnVd;
        mEnVd = nullptr;
    }

}

void SynSt::saveRawVFrs(QList<AVFrame*> rawFrs, const AVCodecContext *deCdcCtx) {
    if (mRawVFrs) {
        mRawVFrs->append(rawFrs);
    }

    mVdCdcCtx = deCdcCtx;
}

void SynSt::saveRawAFrs(QList<AVFrame*> rawFrs, const AVCodecContext *deCdcCtx) {
    if (mRawAFrs) {
        mRawAFrs->append(rawFrs);
    }

    mAuCdcCtx = deCdcCtx;
}

void SynSt::wringFiTra() {
    if (mAlWrTr) {
        if (!*mAlWrTr) {
            mMp4Fi->writeTrailer();
            *mAlWrTr = true;
        }
    }
}

void SynSt::init(const bool enableVd, const bool enableAu) {
    if (!mEnVd) {
        mEnVd = new bool(enableVd);
    }
    if (!mEnAu) {
        mEnAu = new bool(enableAu);
    }
    if (!mImStoSyn) {
        mImStoSyn = new bool;
        *mImStoSyn = false;
    }
    if (!mImStoSynComed) {
        mImStoSynComed = new bool;
        *mImStoSynComed = false;
    }
    if (!mStaingSyn) {
        mStaingSyn = new bool;
        *mStaingSyn = false;
    }
    if (!mStoSyn) {
        mStoSyn = new bool;
        *mStoSyn = false;
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
    if (!mRawVFrs) {
        mRawVFrs = new QQueue<AVFrame*>;
    }
    if (!mRawAFrs) {
        mRawAFrs = new QQueue<AVFrame*>;
    }
    if (!mStoSynCom) {
        mStoSynCom = new bool;
        *mStoSynCom = true;
    }
}

void SynSt::unInit() {
    if (mStoSyn) {
        *mStoSyn = true;
    }
    if (mStaingSyn) {
        *mStaingSyn = false;
    }
}

void SynSt::synVdAuSt() {
    qDebug() << "WrH264AacSt wrVdAuSt thread id: " << QThread::currentThread();
    int VI = 0, AI = 0;
    if (!*mAlWrHe) {
        mMp4Fi->writeHeader();
        *mAlWrHe = true;
    }
    while (true) {
        if (*mImStoSyn) {
            *mImStoSynComed = true;
            break;
        }
        if (*mStoSyn) {
            //qDebug() << "mRawVFrs count: " << mRawVFrs->count() << "mRawAFrs count: " << mRawAFrs->count();
            // 录入缓冲区中剩余的视频
            //if (mRawVFrs->isEmpty()) {
            if ((mRawVFrs->isEmpty()) || (mRawAFrs->isEmpty())) {
                if (!mRawAFrs->isEmpty()) {
                    while (true) {
                        if (mRawAFrs->isEmpty()) {
                            break;
                        }

                        AVFrame *rawAFr = mRawAFrs->dequeue();
                        av_frame_free(&rawAFr);
                    }
                }
                if (!mRawVFrs->isEmpty()) {
                    while (true) {
                        if (mRawVFrs->isEmpty()) {
                            break;
                        }

                        AVFrame *rawVFr = mRawVFrs->dequeue();
                        av_frame_free(&rawVFr);
                    }
                }
                emit sendReVFrsCo(0);
                Log::instance()->info(QString("mRawVFrs count: %1, mRawAFrs count: %2").arg(mRawVFrs->count()).arg(mRawAFrs->count()));
                *mStoSyn = false;
                *mStoSynCom = true;
                break;
            }
            emit sendReVFrsCo(mRawVFrs->count() % 100);
        }

        //qDebug() << "mRawVFrs count: " << mRawVFrs->count() << "mRawAFrs count: " << mRawAFrs->count();
        try {
            if ((*mEnVd) && ((!*mEnAu) || (0 >= av_compare_ts(mH264En->getNextPts(), mH264En->getTB(),
                                                              mAacEn->getNextPts(), mAacEn->getTB())))) {
                //if ((mEnVd) && ((!mEnAu) || (0 >= av_compare_ts(mH264En->getNextPts(), mH264En->getTB(),
                //                                           mAacEn->getNextPts(), mAacEn->getTB())))) {
                //while (true) {
                Log::instance()->info(QString("wrVdAuSt count: %1").arg(mRawVFrs->count()));
                if (!mRawVFrs->isEmpty()) {
                    AVFrame *rawVFr = mRawVFrs->dequeue();
                    emit sendRawVFr(rawVFr, mVdCdcCtx, mH264En, mMp4Fi);
                    //break;
                }

                //QTest::qWait(0.01);
                //}
            } else {
                //while (!mAacEn->getOnAacFrDaCom()) {
                //    if (*mImStoSyn) {
                //        *mImStoSynComed = true;
                //        break;
                //    }
                //    if ((mRawAFrs->isEmpty()) && (*mStoSyn)) {
                //        break;
                //    }

                    if (!mRawAFrs->isEmpty()) {
                        AVFrame *rawAFr = mRawAFrs->dequeue();
                        emit sendRawAFr(rawAFr, mAuCdcCtx, mAacEn, mMp4Fi);
                    }

                //    QTest::qWait(1);
                //}

                //Log::instance()->info(QString("mRawAFrs count: %1").arg(mRawAFrs->count()));
                //Log::instance()->info(QString("Write audio count: %1").arg(AI++));
                //mAacEn->getOnAacFrDaUnCom();
            }
        } catch (screenrecoder::WrH264AacStException &ex) {
            Log::instance()->warn(ex.what());
            //clean();
            *mImStoSynComed = true;
            emit errorSynS();
            return;
        }

        //QTest::qWait(1);
        QThread::msleep(1);
    }

    if (*mEnVd) {
        mH264En->encoder(nullptr, mVdCdcCtx, mMp4Fi, VD, true);
    }
    if (*mEnAu) {
        mAacEn->encoder(nullptr, mAuCdcCtx, mMp4Fi, AU, true);
    }
}
