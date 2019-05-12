// This file deprecated
#ifndef CONTROLVIDEO_H
#define CONTROLVIDEO_H

#include "register.h"
#include "mp4file.h"
#include "videodecoder.h"
#include "h264encoder.h"

class ControlVideo : public CtlVdException {
    Q_OBJECT
public:
    // 如果想抽像成父类 可以把构造函数改成受保护的
    explicit ControlVideo(QObject *parent = nullptr);
    ~ControlVideo();
signals:
    void errorVd();
public slots:
    virtual void init(GenerateFile *file);
    //void init(bool enableVd, bool enableAu);
    virtual void ctlVideo();
    virtual void clean();
    virtual void staReVd();
    virtual void stoReVd();
    virtual void writeVd(StT stt);
    int64_t getNextPts();
    AVRational getTB();
private:
    GenerateFile *mMp4File;
    Decoder *mRawDe;
    Encoder *mH264En;

    // 开始录制和结束录制标识 true 开始录制 false 结束录制
    bool *mStaRe;
};

#endif // CONTROLVIDEO_H
