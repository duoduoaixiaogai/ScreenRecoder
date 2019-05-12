#ifndef REGISTER_H
#define REGISTER_H

// FFmpeg
#ifdef __cplusplus
    extern "C" {
        #include <libavformat/avformat.h>
        #include <libavdevice/avdevice.h>
        #include <libavcodec/avcodec.h>
        #include <libavutil/avutil.h>
        #include <libavfilter/avfilter.h>
        #include <libpostproc/postprocess.h>
        #include <libswresample/swresample.h>
        #include <libswscale/swscale.h>
        #include <libavutil/imgutils.h>
        #include <libavutil/audio_fifo.h>
    }
#endif

// Qt
#include <QDebug>
#include <QThread>
#include <QMessageBox>
#include <QQueue>

// Spdlog
#include "log.h"

// Exception
#include "srexception.h"

// Public
#include "publicbuffer.h"

#endif // REGISTER_H
