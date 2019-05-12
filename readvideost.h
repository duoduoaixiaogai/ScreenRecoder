#ifndef READVIDEOST_H
#define READVIDEOST_H

#include "readst.h"

class ReadVideoSt : public ReadSt {
    Q_OBJECT
public:
    explicit ReadVideoSt(QVector<QPair<QString, QString> > fields, const uint fps = 1,
                         QObject *parent = nullptr);
    ~ReadVideoSt();
    void sendErr();
public slots:
    void startingRe();
    void clean();
signals:
    void errorReVdS();
};

#endif // READVIDEOST_H
