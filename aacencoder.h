#ifndef AACENCODER_H
#define AACENCODER_H

#include "encoder.h"

class AacEncoder : public Encoder {
    Q_OBJECT
public:
    explicit AacEncoder(QObject *parent = nullptr);
    ~AacEncoder();
    void setEnCtx();
    void findEncoder();
    void initEnCtx();
    void initConFrame();
    void sendErr();
    int64_t getNextPts();
    bool getOnAacFrDaCom();
    void getOnAacFrDaUnCom();
protected:
    // 剔除无效数据
    void excInvDa(uint relSamCo);
    void initFifo();
    void addSamToFifo(AVFrame *rawFr);
    void initOpFr(uint frSi);
    void initOpFrEx(AVFrame **outFr, uint frSi);
signals:
    void errorAACEn();
public slots:
    virtual void init(GenerateFile *mp4File);
    virtual void encoder(AVFrame *srcFrame, const AVCodecContext *deCodecCtx, GenerateFile *mp4File, StT stt, bool fluFr = false);
    virtual void clean();
private:
    SwrContext *mSwrCtx;
    AVFrame *mAACFrame;
    AVFrame *mExcedAACFr;
    AVFrame *mOutputFr;
    AVPacket *mAACPkt;
    AVAudioFifo *mAACFifo;

    int mSamplesCount;
    bool *mEnOnAacFrCom;
};

#endif // AACENCODER_H
