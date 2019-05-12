#include "audiodecoder.h"
#include <QDebug>

AudioDecoder::AudioDecoder(QObject *parent) : Decoder(parent) {
    qDebug() << "AudioDecoder structure";
	mDeviceName = "audio=virtual-audio-capturer";
	mFmtCtx = avformat_alloc_context();
   	mInFmt = av_find_input_format("dshow");
}

AudioDecoder::~AudioDecoder() {
   	avformat_close_input(mFmtCtx); 
	avformat_free_context(mFmtCtx);
}

void* AudioDecoder::decoder() {
	
}
