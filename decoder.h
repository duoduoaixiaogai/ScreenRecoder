#ifndef DECODER_H
#define DECODER_H

#include "register.h"
#include "encoder.h"
#include "generatefile.h"

#include <QObject>

class Decoder : public RawDeException {
    Q_OBJECT
public:
    ~Decoder();
    virtual void openInDev();
    virtual void closeInDev();
protected:
    // 分配格式上下文
    void allocFmtCtx();
    // 打开输入设备
    void openInDev(QVector<QPair<QString, QString> > fields);
    // 查找视频流(父类默认查找视频流)
    //virtual void findVideoSt();
    // 查找输入流(父类默认查找视频流)
    void findSt();
    // 查找视频解码器
    //virtual void findVideoDe();
    // 查找解码器
    void findDe();
    // 初始化解码器上下文
    void initDeCtx();
    // 分配存储读取一帧的数据的包和存储解码后原始数据的帧
    void allocPktFrame();
signals:
    void startEncoding(/*AVFrame *srcFrame, AVCodecContext *deCoderCtx*/);
public slots:
    virtual void init();
    virtual void decoder(Encoder *en, GenerateFile *file, StT stt);
    virtual const AVCodecContext* decoder(QQueue<AVFrame*> *rawFrs, StT stt);
    //virtual void decoder(Encoder *h264En, GenerateFile *mp4File);
    virtual void clean();
protected:
    explicit Decoder(QVector<QPair<QString, QString> > fields,
                    const QString format, const QString devName,
                    AVMediaType meT, QObject *parent = nullptr);

    // variable
    AVFormatContext *mFmtCtx;
    AVInputFormat *mInFmt;
    AVCodecContext *mCdcCtx;
    AVCodec *mDeCdc;
    AVFrame *mSrcFrame;
    AVPacket *mSrcPkt;
    AVCodecParameters *mCdcPars;

    const QString mFmt;
    const QString mDevName;
    QVector<QPair<QString, QString> > mFields;
    unsigned int mIdxSt;
    //unsigned int mVdoIdxSt;
    AVMediaType mMeT;
    //test
    int vI = 0, aI = 0;
};

#endif // DECODER_H
