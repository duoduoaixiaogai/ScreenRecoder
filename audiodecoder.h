#ifndef AUDIODECODER_H
#define AUDIODECODER_H

#include "decoder.h"

class AudioDecoder : public Decoder {
    Q_OBJECT
public:
    explicit AudioDecoder(QVector<QPair<QString, QString> > fields,
                        const QString format = "dshow",
                        const QString devName = "audio=virtual-audio-capturer",
                        AVMediaType meT = AVMEDIA_TYPE_AUDIO, QObject *parent = nullptr) :
        Decoder(fields, format, devName, meT, parent) {}

    ~AudioDecoder() {}
};

#endif // AUDIODECODER_H
