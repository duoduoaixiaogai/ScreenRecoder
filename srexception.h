#ifndef SREXCEPTION_H
#define SREXCEPTION_H

#include <QObject>
#include <exception>

class SRException : public QObject {
    Q_OBJECT
public:
    explicit SRException(QObject *parent = nullptr);
    virtual ~SRException();
    virtual void sendErr();
signals:
    void error();
private:

    // variable
};

class AACEnException : public SRException {
    Q_OBJECT
public:
    explicit AACEnException(QObject *parent = nullptr);
    void sendErr();
signals:
    void errorAACEn();
};


class RawDeException : public SRException {
    Q_OBJECT
public:
    explicit RawDeException(QObject *parent = nullptr);
    void sendErr();
signals:
    void errorRawDe();
};

class H264EnException : public SRException {
    Q_OBJECT
public:
    explicit H264EnException(QObject *parent = nullptr);
    void sendErr();
signals:
    void errorH264En();
};

class Mp4FiException : public SRException {
    Q_OBJECT
public:
    explicit Mp4FiException(QObject *parent = nullptr);
    void sendErr();
signals:
    void errorMp4Fi();
};

class CtlAuException : public SRException {
    Q_OBJECT
public:
    explicit CtlAuException(QObject *parent = nullptr);
    void sendErr();
signals:
    void errorCtlAu();
};

class CtlVdException : public SRException {
    Q_OBJECT
public:
    explicit CtlVdException(QObject *parent = nullptr);
    void sendErr();
signals:
    void errorCtlVd();
};

namespace screenrecoder {
    class RawDeException : public std::exception {
    public:
        explicit RawDeException(const QString errMsg);
        //char const* what() const;
    };

    class WrH264AacStException : public std::exception {
    public:
        explicit WrH264AacStException(const QString errMsg);
    };

    class H264EnException : public WrH264AacStException{
    public:
        explicit H264EnException(const QString errMsg);
        //char const* what() const;
    };

    class AacEnException : public WrH264AacStException{
    public:
        explicit AacEnException(const QString errMsg);
        //char const* what() const;
    };

    class Mp4FiException : public std::exception {
    public:
        explicit Mp4FiException(const QString errMsg);
        //char const* what() const;
    };

}

#endif // SREXCEPTION_H
