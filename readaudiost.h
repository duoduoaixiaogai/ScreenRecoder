#ifndef READAUDIOST_H
#define READAUDIOST_H

#include "readst.h"

class AudioDecoder;

class ReadAudioSt : public ReadSt {
    Q_OBJECT
public:
    explicit ReadAudioSt(QVector<QPair<QString, QString> >fields, const uint fps = 1,
                         QObject *parent = nullptr);
    ~ReadAudioSt();
    void sendErr();
signals:
    void errorReAuS();
public slots:
    void startingRe();
    void clean();
};

#endif // READAUDIOST_H
