#ifndef SYNST_H
#define SYNST_H

#include "register.h"

class Mp4File;
class H264Encoder;
class AacEncoder;
class Encoder;
class Decoder;
class GenerateFile;

class SynSt : public SRException {
    Q_OBJECT
public:
    explicit SynSt(QObject *parent = nullptr);
    virtual ~SynSt();
    bool alStaSyn();
    bool stoSynCom();
    bool imStoSynComed();
signals:
    void sendRawVFr(AVFrame* rawFr, const AVCodecContext *deCdcCtx, Encoder *enCdr, GenerateFile *fi);
    void sendRawAFr(AVFrame* rawFrs, const AVCodecContext *deCdcCtx, Encoder *enCdr, GenerateFile *fi);
    void errorSynS();
    void sendReVFrsCo(const int reVFrsCo);
public slots:
    virtual void startingSyn(const bool enableVd, const bool enableAu);
    virtual void pausingSyn();
    virtual void imStoSyn();
    virtual void clean();
    virtual void saveRawVFrs(QList<AVFrame*> rawFrs, const AVCodecContext *deCdcCtx);
    virtual void saveRawAFrs(QList<AVFrame*> rawFrs, const AVCodecContext *deCdcCtx);
    virtual void wringFiTra();
protected:
    void init(const bool enableVd, const bool enableAu);
    void unInit();
    void synVdAuSt();
    // variable
    QQueue<AVFrame*> *mRawVFrs, *mRawAFrs;

    bool *mStoSyn, *mStaingSyn, *mInited, *mAlWrHe, *mAlWrTr, *mImStoSyn, *mImStoSynComed, *mStoSynCom;
    const bool *mEnVd, *mEnAu;

    const AVCodecContext *mVdCdcCtx, *mAuCdcCtx;

    Mp4File *mMp4Fi;
    H264Encoder *mH264En;
    AacEncoder *mAacEn;
};

#endif // SYNST_H
