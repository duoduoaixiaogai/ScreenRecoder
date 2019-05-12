// This file deprecated
#ifndef CONTROLAUDIO_H
#define CONTROLAUDIO_H

#include "register.h"
#include "generatefile.h"
#include "audiodecoder.h"
#include "aacencoder.h"

class ControlAudio : public CtlAuException {
    Q_OBJECT
public:
    explicit ControlAudio(QObject *parent = nullptr);
    ~ControlAudio();
signals:
    void errorAu();
public slots:
    virtual void init(GenerateFile *file);
    virtual void ctlAudio();
    virtual void clean();
    virtual void staReAu();
    virtual void stoReAu();
    virtual void writeAu(StT stt);
    int64_t getNextPts();
    AVRational getTB();
private:
    GenerateFile *mMp4File;
    AudioDecoder *mAuDe;
    AacEncoder *mAACEn;

    bool *mStaRe;
};

#endif // CONTROLAUDIO_H
