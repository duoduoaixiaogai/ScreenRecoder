#ifndef PUBLICBUFFER_H
#define PUBLICBUFFER_H

#include "register.h"
#include <QMutex>
#include <QWaitCondition>

class PublicBuffer : public QObject {
public:
    ~PublicBuffer();
    static PublicBuffer* instance();
    void putRawVFr(AVFrame *rawFr);
    void putRawAFr(AVFrame *rawFr);
    AVFrame* getRawVFr();
    AVFrame* getRawAFr();
    bool isRawVFrsBufEmpty();
    bool isRawAFrsBufEmpty();
    int getRawVFrsBufMemCou();
    int getRawAFrsBufMemCou();
    void setRawVCdcCtx(const AVCodecContext *cdcCtx);
    void setRawACdcCtx(const AVCodecContext *cdcCtx);
    const AVCodecContext* getRawVCdcCtx() const;
    const AVCodecContext* getRawACdcCtx() const;
    void wakeWrVTh();
    void wakeWrATh();
    QString getErrStr(int errNum);
    void clean();
private:
    PublicBuffer(QObject *parent = nullptr);

    //variable
    static PublicBuffer *mInstance;

    QQueue<AVFrame*> mRawVFrsBuf, mRawAFrsBuf;

    const AVCodecContext *mRawVCdcCtx, *mRawACdcCtx;

    QMutex mMutex;
    QWaitCondition mRawVFrsBufNotFull, mRawVFrsBufNotEmpty, mRawAFrsBufNotFull, mRawAFrsBufNotEmpty;
    const int mMaxBufMemsCou, mTimeout;
};

#endif // PUBLICBUFFER_H
