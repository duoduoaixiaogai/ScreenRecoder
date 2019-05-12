#ifndef CONTROLTHREAD_H
#define CONTROLTHREAD_H

/*
    This file is deprecated
*/
#include "controlfile.h"
#include <QThread>

class ControlThread : public QObject {
    Q_OBJECT
public:
    // 如果想把类抽象成父类 可以把构造函数改成受保护的
    explicit ControlThread(bool enableVd, bool enableAu, QObject *parent = nullptr);
    ~ControlThread();
signals:
    void startingRec();
    void stopingRec();
    void initFile(bool enableVd, bool enableAu, ControlThread *ctlTh);
public slots:
    virtual void startedRecoder();
    virtual void stopedRecoder();
    virtual void quitThread();
private slots:
    void initingFile();
private:
    ControlFile *mCtlFile;

    // 是否录制视频和音频(默认情况下都录制)
    bool mEnableVd, mEnableAu;

    QThread mThread;
};

#endif // CONTROLTHREAD_H
