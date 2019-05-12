#ifndef WRITEVDAUST_H
#define WRITEVDAUST_H

#include "register.h"

class WriteSt : public SRException {
    Q_OBJECT
public:
    virtual ~WriteSt();
public slots:
    virtual void startingWr(const bool enableVd, const bool enableAu);
    virtual void pausingWr();
    virtual void imStoWr();
    virtual bool alStaWr();
    virtual bool stoWrCom();
    virtual void wringFiTra();
    virtual bool imStoWrComed();
    virtual void clean();
    virtual void flush();
protected:
    explicit WriteSt(QObject *parent = nullptr);
    void init(const bool enableVd, const bool enableAu);
    void uninit();

    // variable
    const AVCodecContext *mDeVdCtx, *mDeAuCtx;

    bool *mStoWr, *mStaingWr, *mInited, *mAlWrHe, *mAlWrTr, *mImStoWr, *mImStoWrComed, *mStoWrCom;
    const bool *mEnVd, *mEnAu;
};

#endif // WRITEVDAUST_H
