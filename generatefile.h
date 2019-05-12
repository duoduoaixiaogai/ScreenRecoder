#ifndef GENERATEFILE_H
#define GENERATEFILE_H

#include "register.h"

QT_BEGIN_NAMESPACE
#include <QFile>
QT_END_NAMESPACE

// 流类型
enum StT {
    VD = 0,
    AU,
};

class GenerateFile : public SRException {
    Q_OBJECT
public:
    virtual ~GenerateFile();
    // 打开输出设备
    void openOutDev();
    // 初始化AVIOContext以访问指定的资源
    void initAVIO();
    // 创建视频流
    void createVdoSt();
    // 创建音频流
    void createAuSt();
    // 创建流
    AVStream* createSt();
signals:
    // 开始解码数据
    void startDecode();
    // 将流传送给编码对象(avcodec_alloc_context3这个函数在程序这里不能使用，会造成程序崩溃暂时没有找到问题的原因)
    void sendStObj(AVStream *genFileSt);
public slots:
    virtual void init(bool enableVd, bool enableAu) = 0;
    virtual void generateFile(AVPacket *enPkt, const AVRational *timeBase, StT stt) = 0;
    virtual void generatePcmFi(AVSampleFormat samFmt, AVFrame *rawFr, uint relSamCo) {}
    virtual void clean() = 0;
    virtual void writeHeader() = 0;
    virtual void writeTrailer() = 0;
    // 初始化输出流中的编解码器上下文信息
    virtual void initStCdcCtx(AVCodecContext *enCdcCtx, StT stT) = 0;
protected:
    explicit GenerateFile(const QString pcmFiNa, const QString fileName, QObject *parent = nullptr);

    // variable
    AVFormatContext *mFmtCtx;
    AVOutputFormat *mOutFmt;
    AVStream *mVdoSt;
    AVStream *mAuSt;

    const QString mOutFile;
    bool mWriteHead;
    int mAVIOSu;
    QFile mOutFi;
};

#endif // GENERATEFILE_H
