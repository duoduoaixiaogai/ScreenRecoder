#ifndef CONTROLTHREADEX_H
#define CONTROLTHREADEX_H

#include "register.h"

class WriteSt;
class ReadSt;

class ControlThreadEx : public SRException {
    Q_OBJECT
public:
    explicit ControlThreadEx(const bool enableVd = true, const bool enableAu = true, QObject *parent = nullptr);
    ~ControlThreadEx();
signals:
    void startReVd();
    void startReAu();
    void startWrSt(const bool enableVd, const bool enableAu);
    void transReVFrsCo(const int reVFrsCo);
public slots:
    void closeApp();
    void startRe();
    void stopRe();
    void imStoRe();
    void pauseRe();
    void handlingErrs();
private:
    WriteSt *mWrH264AacS;
    ReadSt *mReVdS;
    ReadSt *mReAuS;

    QThread mWrStTh;
    QThread mReVdSTh;
    QThread mReAuSTh;

    // 是否录制视频和音频(默认情况下都录制)
    const bool mEnableVd, mEnableAu;

    bool mPaused;
};
#endif // CONTROLTHREADEX_H
