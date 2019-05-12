//This file deprecated
#include "controlfile.h"
#include "mp4file.h"
#include "controlaudio.h"
#include "controlvideo.h"
#include "controlthread.h"

ControlFile::ControlFile(QObject *parent) : QObject(parent), mMp4File(nullptr), mCtlAu(nullptr), mCtlVd(nullptr), mStoRe(nullptr) {
    mMp4File = new Mp4File;

    mCtlAu = new ControlAudio;
    mCtlVd = new ControlVideo;

    connect(this, &ControlFile::sendFiToAu, mCtlAu, &ControlAudio::init, Qt::BlockingQueuedConnection);
    connect(this, &ControlFile::sendFiToVd, mCtlVd, &ControlVideo::init, Qt::BlockingQueuedConnection);

    connect(mCtlAu, &ControlAudio::errorAu, this, &ControlFile::errorTh);
    connect(mCtlVd, &ControlVideo::errorVd, this, &ControlFile::errorTh);
    connect(dynamic_cast<Mp4File*>(mMp4File), &Mp4File::errorMp4Fi, this, &ControlFile::errorTh);
}

ControlFile::~ControlFile() {
    if (mMp4File) {
        delete mMp4File;
        mMp4File = nullptr;
    }
    if (mCtlAu) {
        delete mCtlAu;
        mCtlAu = nullptr;
    }
    if (mCtlVd) {
        delete mCtlVd;
        mCtlVd = nullptr;
    }
    if (mStoRe) {
        delete mStoRe;
        mStoRe = nullptr;
    }
}

void ControlFile::init(bool enableVd, bool enableAu) {
    qDebug() << "ControlFile::init " << QThread::currentThreadId();


    mEnAu = enableAu;
    mEnVd = enableVd;

    mStoRe = new bool;
    *mStoRe = false;

    mMp4File->init(enableVd, enableAu);

    if (mEnVd) {
        emit sendFiToVd(mMp4File);
    }
    if (mEnAu) {
        emit sendFiToAu(mMp4File);
    }
}

void ControlFile::ctlFile() {
    while (true) {
        if (*mStoRe) {
            break;
        }

        if (mEnVd && (!mEnAu || 0 >= av_compare_ts(mCtlVd->getNextPts(), mCtlVd->getTB(),
                                                   mCtlAu->getNextPts(), mCtlAu->getTB()))) {
          mCtlVd->writeVd(VD);
        } else {
          mCtlAu->writeAu(AU);
        }

        // 不建议延迟太大 延迟太大会缩短录制的时间(可能想录制15秒，实际出来的只有8秒左右, 当延迟在10ms左右时)
        QTest::qWait(0.01);
    }
}

void ControlFile::clean() {
    mMp4File->clean();
    if (mEnVd) {
        mCtlVd->clean();
    }
    if (mEnAu) {
        mCtlAu->clean();
    }
}

void ControlFile::staReFi() {
    if (mStoRe) {
        *mStoRe = false;
    }
}

void ControlFile::stoReFi() {
    if (mStoRe) {
        *mStoRe = true;
    }
}

void ControlFile::writeTrailer() {
    mMp4File->writeTrailer();
}
