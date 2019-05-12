#ifndef WRITESTEX_H
#define WRITESTEX_H

#include "register.h"
#include "h264encoder.h"
#include "aacencoder.h"
#include "mp4file.h"

class WriteStEx : public SRException {
    Q_OBJECT
public:
    explicit WriteStEx(QObject *parent = nullptr);
    virtual ~WriteStEx();
public slots:
    virtual void writeRawFr(AVFrame* rawFr, const AVCodecContext *deCdcCtx, Encoder *enCdr, GenerateFile *fi);
};

#endif // WRITESTEX_H
