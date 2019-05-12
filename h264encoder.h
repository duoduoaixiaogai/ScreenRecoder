#ifndef H264ENCODER_H
#define H264ENCODER_H

#include "encoder.h"

// 希望编码后视频图像的宽和高
enum EnWH {
    //ENW = 480,
    //ENH = 270,
    ENW = 1920,
    ENH = 1080,
};

class H264Encoder : public Encoder {
    Q_OBJECT
public:
    explicit H264Encoder(QObject *parent = nullptr);
    ~H264Encoder();
    void setEnCtx();
    void findEncoder();
    void initEnCtx();
    void initConFrame();
    int64_t getNextPts();
    AVRational getTB();
    void sendErr();
signals:
    void errorH264En();
public slots:
    virtual void init(GenerateFile *mp4File);
    virtual void encoder(AVFrame *srcFrame, const AVCodecContext *deCodecCtx, GenerateFile *mp4File, StT stt, bool fluFr = false);
    virtual void clean();
private:
    SwsContext *mSwsCtx;
};

#endif // H264ENCODER_H
