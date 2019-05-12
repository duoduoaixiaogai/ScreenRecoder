/*
    This file is deprecated
*/
#include "controlthread.h"

ControlThread::ControlThread(bool enableVd, bool enableAu, QObject *parent) : QObject(parent), mCtlFile(nullptr), mEnableVd(enableVd),
    mEnableAu(enableAu) {
    qDebug() << "ControlThread::ControlThread " << QThread::currentThreadId();

    mCtlFile = new ControlFile;
    //mCtlVideo = new ControlVideo;
    //mCtlAudio = new ControlAudio;

    mCtlFile->moveToThread(&mThread);
    //mCtlVideo->moveToThread(&mThread);
    //mCtlAudio->moveToThread(&mThread);

    // public slots
    connect(this, &ControlThread::startingRec, this, &ControlThread::initingFile);
    // 写文件应该是一个单独的对象，它是视频和音频都共同操作的对象
    connect(this, &ControlThread::initFile, mCtlFile, &ControlFile::init);
    // 执行
    connect(this, &ControlThread::startingRec, mCtlFile, &ControlFile::ctlFile);
    // 开始录制
    connect(this, &ControlThread::startingRec, mCtlFile, &ControlFile::staReFi);
    // 写文件尾(流程顺序不可改变，写文件尾操作一定要在清理之前执行)
    connect(this, &ControlThread::stopingRec, mCtlFile, &ControlFile::writeTrailer);
    // 清理
    connect(this, &ControlThread::stopingRec, mCtlFile, &ControlFile::clean);
    // 结束录制
    connect(this, &ControlThread::stopingRec, mCtlFile, &ControlFile::stoReFi);

    // 清理堆资源
    connect(&mThread, &QThread::finished, mCtlFile, &ControlFile::deleteLater);

    // Qt::BlockingQueuedConnection 注意这个连接方式的死锁问题(提前在录制线程初始化异常对象) ControlFile::init线程中初始化
    connect(mCtlFile, &ControlFile::errorTh, this, &ControlThread::quitThread, Qt::BlockingQueuedConnection);

    mThread.start();
}

ControlThread::~ControlThread() {
    if (!mThread.isFinished()) {
        mThread.exit(-1);
        mThread.wait(1000);
    }

    if (mThread.isRunning()) {
        mThread.terminate();
        mThread.wait();
    }
}

void ControlThread::startedRecoder() {
    emit startingRec();
}

void ControlThread::stopedRecoder() {
    emit stopingRec();
}

void ControlThread::quitThread() {
    mCtlFile->stoReFi();
    mCtlFile->clean();

    // 如果有退出线程需求的话 可以在这里退出线程

    qDebug() << "ScreenRecoder thread quited";
    QMessageBox::critical(nullptr, "Critical", "Check log, find error!");
}

void ControlThread::initingFile() {
    emit initFile(mEnableVd, mEnableAu, this);
}
