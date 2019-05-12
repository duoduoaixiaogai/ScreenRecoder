#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include "register.h"

class ConfigFile : public SRException {
    Q_OBJECT
public:
    ~ConfigFile();
    static ConfigFile* instance();
    ReadVSCfg getReadVSCfg();
    ReadASCfg getReadASCfg();
    WriteFiCfg getWriteFiCfg();
private:
    explicit ConfigFile(QObject *parent = nullptr);

    //variable
    static ConfigFile *mInstance;
};

#endif // CONFIGFILE_H
