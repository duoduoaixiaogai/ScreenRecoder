#ifndef MP4FILE_H
#define MP4FILE_H

#include "generatefile.h"

class Mp4File : public GenerateFile {
    Q_OBJECT
public:
    explicit Mp4File(const QString pcmFiNa = QCoreApplication::applicationDirPath() + QString("/ScrrenRecoder.pcm"), const QString fileName = QCoreApplication::applicationDirPath() + QString("/ScrrenRecoder.mp4"), QObject *parent = nullptr);
    ~Mp4File();
    void openOutDev();
    void initAVIO();
    void createVdoSt();
    void createAuSt();
    AVStream* createSt();
    void sendErr();
signals:
    void errorMp4Fi();
public slots:
    virtual void init(bool enableVd, bool enableAu);
    virtual void generateFile(AVPacket *enPkt, const AVRational *timeBase, StT stt);
    virtual void generatePcmFi(AVSampleFormat samFmt, AVFrame *rawFr, uint relSamCo);
    virtual void clean();
    virtual void writeHeader();
    virtual void writeTrailer();
    virtual void initStCdcCtx(AVCodecContext *enCdcCtx, StT stT);
protected:
};

#endif // MP4FILE_H
