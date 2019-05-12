#ifndef WRH264AACST_H
#define WRH264AACST_H

#include "writest.h"

class H264Encoder;
class AacEncoder;
class Mp4File;

class WrH264AacSt : public WriteSt {
    Q_OBJECT
public:
    explicit WrH264AacSt(QObject *parent = nullptr);
    ~WrH264AacSt();
    void sendErr();
signals:
    void errorWrH264AacS();
    void sendReVFrsCo(const int reVFrsCo);
public slots:
    void startingWr(const bool enableVd, const bool enableAu);
    void pausingWr();
    void wringFiTra();
    void clean();
    void flush();
protected:
    void init(const bool enableVd, const bool enableAu);
    void uninit();
    void wrVdAuSt();
private:
    H264Encoder *mH264En;
    AacEncoder *mAacEn;
    Mp4File *mMp4Fi;
};

#endif // WRH264AACST_H
