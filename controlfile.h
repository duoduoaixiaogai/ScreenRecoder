//This file deprecated
#ifndef CONTROLFILE_H
#define CONTROLFILE_H

#include "register.h"

class GenerateFile;
class ControlVideo;
class ControlAudio;
class ControlThread;

class ControlFile : public QObject {
    Q_OBJECT
public:
    // 如果想抽象成父类可以把构造函数改成受保护的
    explicit ControlFile(QObject *parent = nullptr);
    ~ControlFile();
signals:
    void sendFiToVd(GenerateFile *file);
    void sendFiToAu(GenerateFile *file);
    void errorTh();
public slots:
    virtual void init(bool enableVd, bool enableAu);
    virtual void ctlFile();
    virtual void clean();
    virtual void staReFi();
    virtual void stoReFi();
    virtual void writeTrailer();
private:
    GenerateFile *mMp4File;

    ControlAudio *mCtlAu;
    ControlVideo *mCtlVd;

    bool *mStoRe;
    bool mEnVd, mEnAu;
};

#endif // CONTROLFILE_H
