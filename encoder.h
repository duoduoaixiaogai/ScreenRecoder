#ifndef ENCODER_H
#define ENCODER_H

#include "register.h"
#include "generatefile.h"

#include <QObject>

// 希望编码后视频图像的宽和高
//enum EnWH {
//    ENW = 1920,
//    ENH = 1080,
//};

//这个类没有抽象好 子类还是会继承一些不想要的成员属性
class Encoder : public SRException {
    Q_OBJECT
public:
    virtual ~Encoder();
    // 设置编码器上下文
    void setEnCtx();
    // 查找编码器
    void findEncoder();
    // 初始化编码器上下文
    void initEnCtx();
    // 初始化转换帧
    void initConFrame();
    int64_t getNextPts();
    AVRational getTB();
signals:
    void writeFile(/*AVPacket *enPktm, const AVRational *timeBase*/);
    void sendEnCdcCtx(AVCodecContext *enCdcCtx);
public slots:
    virtual void init(GenerateFile *mp4File) = 0;
    virtual void encoder(AVFrame *srcFrame, const AVCodecContext *deCodecCtx, GenerateFile *mp4File, StT stt, bool fluFr = false) = 0;
    virtual void clean() = 0;
protected:
    explicit Encoder(QObject *parent = nullptr);

    // variable
    AVCodecContext *mCdcCtx;
    AVCodec *mEnCdc;
    //AVFrame *mYUVFrame;
    //SwsContext *mSwsCtx;
    //AVPacket *mYUVPkt;

    AVFrame *mRawFrame;
    AVPacket *mEnedPkt;
    uint8_t *mOutBuf;

    int64_t mNextPts;
    int mPicSize;
    int mDstNbSamples;
    int mMaxDstNbSam;

    bool mInitStCdcCtx;
};

#endif // ENCODER_H
