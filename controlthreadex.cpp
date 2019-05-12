#include "controlthreadex.h"
#include "wrh264aacst.h"
#include "readvideost.h"
#include "readaudiost.h"
#include "writevideost.h"
#include "writeaudiost.h"
#include "synst.h"
#include "publicbuffer.h"

ControlThreadEx::ControlThreadEx(const bool enableVd, const bool enableAu, QObject *parent) :
    SRException(parent), mWrH264AacS(nullptr), mReVdS(nullptr), mReAuS(nullptr), mEnableVd(enableVd),
    mEnableAu(enableAu), mPaused(false) {

    QVector<QPair<QString, QString> > fieldsVd, fieldsAu;
    QPair<QString, QString> field;

    //field.first = "video_size";
    //field.second = "hd1080";

    field.first = "framerate";
    field.second = "60";
    fieldsVd.append(field);

    mWrH264AacS = new WrH264AacSt;
    mReVdS = new ReadVideoSt(fieldsVd);
    mReAuS = new ReadAudioSt(fieldsAu);

    mWrH264AacS->moveToThread(&mWrStTh);
    mReVdS->moveToThread(&mReVdSTh);
    mReAuS->moveToThread(&mReAuSTh);

    qRegisterMetaType<QList<AVFrame*> >();
    qRegisterMetaType<const AVCodecContext*>();

    connect(this, &ControlThreadEx::startReVd, dynamic_cast<ReadVideoSt*>(mReVdS), &ReadVideoSt::startingRe);
    connect(this, &ControlThreadEx::startReAu, dynamic_cast<ReadAudioSt*>(mReAuS), &ReadAudioSt::startingRe);
    connect(this, &ControlThreadEx::startWrSt, dynamic_cast<WrH264AacSt*>(mWrH264AacS), &WrH264AacSt::startingWr);
    connect(dynamic_cast<WrH264AacSt*>(mWrH264AacS), &WrH264AacSt::errorWrH264AacS, this, &ControlThreadEx::handlingErrs);
    connect(dynamic_cast<ReadVideoSt*>(mReVdS), &ReadVideoSt::errorReVdS, this, &ControlThreadEx::handlingErrs);
    connect(dynamic_cast<ReadAudioSt*>(mReAuS), &ReadAudioSt::errorReAuS, this, &ControlThreadEx::handlingErrs);

    //connect(dynamic_cast<WrH264AacSt*>(mWrH264AacS), &WrH264AacSt::sendReVFrsCo, this, &ControlThreadEx::transReVFrsCo);

    mWrStTh.start();
    mReVdSTh.start();
    mReAuSTh.start();
}

ControlThreadEx::~ControlThreadEx() {
    //if (!mWrStTh.isFinished()) {
    //    mWrStTh.quit();
    //    mWrStTh.wait(1000);
    //}

    //if (mWrStTh.isRunning()) {
    //    mWrStTh.terminate();
    //    mWrStTh.wait();
    //}

    //if (!mReVdSTh.isFinished()) {
    //    mReVdSTh.quit();
    //    mReVdSTh.wait(1000);
    //}

    //if (mReVdSTh.isRunning()) {
    //    mReVdSTh.terminate();
    //    mReVdSTh.wait();
    //}

    //if (!mReAuSTh.isFinished()) {
    //    mReAuSTh.quit();
    //    mReAuSTh.wait(1000);
    //}

    //if (mReAuSTh.isRunning()) {
    //    mReAuSTh.terminate();
    //    mReAuSTh.wait();
    //}

    if (mReVdS) {
        delete mReVdS;
        mReVdS = nullptr;
    }

    if (mReAuS) {
        delete mReAuS;
        mReAuS = nullptr;
    }

    if (mWrH264AacS) {
        delete mWrH264AacS;
        mWrH264AacS = nullptr;
    }
}

void ControlThreadEx::startRe() {
    if (mEnableVd) {
        if (!mReVdSTh.isRunning()) {
            mReVdSTh.start();
        }
        emit startReVd();
    }

    if (mEnableAu) {
        if (!mReAuSTh.isRunning()) {
            mReAuSTh.start();
        }
        emit startReAu();
    }

    if (!mWrStTh.isRunning()) {
        mWrStTh.start();
    }
    emit startWrSt(mEnableVd, mEnableAu);

    mPaused = false;
}

void ControlThreadEx::stopRe() {
    if (!mPaused) {
        pauseRe();
    }

    mWrH264AacS->flush();
    mWrH264AacS->wringFiTra();

    if (mEnableAu) {
        mReAuS->clean();
    }
    if (mEnableVd) {
        mReVdS->clean();
    }
    mWrH264AacS->clean();
    PublicBuffer::instance()->clean();
}

void ControlThreadEx::imStoRe() {
    if (mEnableVd) {
        mReVdS->imStoRe();
    }
    if (mEnableAu) {
        mReAuS->imStoRe();
    }
    mWrH264AacS->imStoWr();
}

void ControlThreadEx::pauseRe() {
    if (mEnableVd) {
        mReVdS->pausingRe();
        while (true) {
            if (mReVdS->stoReCom()) {
                break;
            }
            QThread::msleep(100);
        }
    }

    if (mEnableAu) {
        mReAuS->pausingRe();
        while (true) {
            if (mReAuS->stoReCom()) {
                break;
            }
            QThread::msleep(100);
        }
    }

    mWrH264AacS->pausingWr();
    while (true) {
        if (mWrH264AacS->stoWrCom()) {
            break;
        }
        QThread::msleep(100);
    }

    mPaused = true;
}

void ControlThreadEx::closeApp() {
    if (mEnableVd) {
        if (mReVdS->alStaRe()) {
            mReVdS->pausingRe();
            while (true) {
                if (mReVdS->stoReCom()) {
                    break;
                }
            }
        }
        mReVdS->clean();
    }
    if (mEnableAu) {
        if (mReAuS->alStaRe()) {
            mReAuS->pausingRe();
            while (true) {
                if (mReAuS->stoReCom()) {
                    break;
                }
            }
        }
        mReAuS->clean();
    }
    if (mWrH264AacS->alStaWr()) {
        mWrH264AacS->pausingWr();
        while (true) {
            if (mWrH264AacS->stoWrCom()) {
                break;
            }
        }
    }
    mWrH264AacS->clean();
}

void ControlThreadEx::handlingErrs() {
    //imStoRe();

    //if (mEnableVd) {
    //    while (true) {
    //        if (mReVdS->imStoReComed()) {
    //            mReVdS->clean();
    //            break;
    //        }
    //        QThread::msleep(100);
    //    }
    //}
    //if (mEnableAu) {
    //    while (true) {
    //        if (mReAuS->imStoReComed()) {
    //            mReAuS->clean();
    //            break;
    //        }
    //        QThread::msleep(100);
    //    }
    //}
    //while (true) {
    //    if (mWrH264AacS->imStoWrComed()) {
    //        mWrH264AacS->clean();
    //        break;
    //    }
    //    QThread::msleep(100);
    //}

    QMessageBox::critical(nullptr, "Critical", "Check log, find error!");
}
