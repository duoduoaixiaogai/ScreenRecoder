#ifndef READST_H
#define READST_H

#include "register.h"

class Decoder;

class ReadSt : public SRException {
    Q_OBJECT
public:
    explicit ReadSt(const uint fps, QObject *parent = nullptr);
    virtual ~ReadSt();
protected:
    void init();
    void unInit();
public slots:
    virtual void startingRe();
    virtual void pausingRe();
    virtual void imStoRe();
    virtual bool imStoReComed();
    virtual bool alStaRe();
    virtual bool stoReCom();
    virtual void clean();
protected:
    QQueue<AVFrame*> *mRawFrs;
    const AVCodecContext *mDeCdcCtx;
    Decoder *mReS;
    bool *mStoRe;
    bool *mStaingRe;
    bool *mInited;
    bool *mPaused;
    bool *mImStoRe;
    bool *mImStoReComed;
    bool *mStoReCom;
    const uint mFps;
};

Q_DECLARE_METATYPE(QList<AVFrame*>)
Q_DECLARE_METATYPE(const AVCodecContext*)

#endif // READST_H
