#ifndef WRITEAUST_H
#define WRITEAUST_H

#include "writestex.h"

class WriteAudioSt : public WriteStEx {
    Q_OBJECT
public:
    explicit WriteAudioSt(QObject *parent = nullptr);
    ~WriteAudioSt();
public slots:
    void writeRawFr(AVFrame* rawFr, const AVCodecContext *deCdcCtx, Encoder *enCdr, GenerateFile *fi);
};

#endif // WRITEAUST_H
