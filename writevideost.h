#ifndef WRITEVDST_H
#define WRITEVDST_H

#include "writestex.h"

class WriteVideoSt : public WriteStEx {
    Q_OBJECT
public:
    explicit WriteVideoSt(QObject *parent = nullptr);
    ~WriteVideoSt();
public slots:
    void writeRawFr(AVFrame* rawFr, const AVCodecContext *deCdcCtx, Encoder *enCdr, GenerateFile *fi);
};

#endif // WRITEVDST_H
