#ifndef VIDEODECODER_H
#define VIDEODECODER_H

#include "decoder.h"

class VideoDecoder : public Decoder {
    Q_OBJECT
public:
    explicit VideoDecoder(QVector<QPair<QString, QString> > fields,
                          const QString format = "dshow",
                          const QString devName = "video=screen-capture-recorder",
                          AVMediaType meT = AVMEDIA_TYPE_VIDEO, QObject *parent = nullptr) :
        Decoder(fields, format, devName, meT, parent) {}
    ~VideoDecoder() {}
};

#endif // VIDEODECODER_H
