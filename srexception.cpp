#include "srexception.h"

SRException::SRException(QObject *parent) : QObject(parent) {

}

SRException::~SRException() {

}


void SRException::sendErr() {
    emit error();
}


AACEnException::AACEnException(QObject *parent) : SRException(parent) {

}

void AACEnException::sendErr() {
    emit errorAACEn();
}

RawDeException::RawDeException(QObject *parent) : SRException(parent) {

}

void RawDeException::sendErr() {
    emit errorRawDe();
}

H264EnException::H264EnException(QObject *parent) : SRException(parent) {

}

void H264EnException::sendErr() {
    emit errorH264En();
}

Mp4FiException::Mp4FiException(QObject *parent) : SRException(parent) {

}

void Mp4FiException::sendErr() {
    emit errorMp4Fi();
}

CtlAuException::CtlAuException(QObject *parent) : SRException(parent) {

}

void CtlAuException::sendErr() {
    emit errorCtlAu();
}

CtlVdException::CtlVdException(QObject *parent) : SRException(parent) {

}

void CtlVdException::sendErr() {
    emit errorCtlVd();
}

namespace screenrecoder {
    RawDeException::RawDeException(const QString errMsg) : std::exception(errMsg.toLatin1().data()) {}
    //char const* RawDeException::what() const {}
    H264EnException::H264EnException(const QString errMsg) : WrH264AacStException(errMsg.toLatin1().data()) {}
    //char const* H264EnException::what() const {}
    AacEnException::AacEnException(const QString errMsg) : WrH264AacStException(errMsg.toLatin1().data()) {}
    //char const* AacEnException::what() const {}
    Mp4FiException::Mp4FiException(const QString errMsg) : std::exception(errMsg.toLatin1().data()) {}
    //char const* Mp4FiException::what() const {}
    WrH264AacStException::WrH264AacStException(const QString errMsg) :std::exception(errMsg.toLatin1().data()) {}
}
