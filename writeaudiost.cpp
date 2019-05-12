#include "writeaudiost.h"

WriteAudioSt::WriteAudioSt(QObject *parent) : WriteStEx(nullptr) {

}

WriteAudioSt::~WriteAudioSt() {

}

void WriteAudioSt::writeRawFr(AVFrame* rawFr, const AVCodecContext *deCdcCtx, Encoder *enCdr, GenerateFile *fi) {
   Mp4File *mp4Fi = dynamic_cast<Mp4File*>(fi);
   AacEncoder *aacEn = dynamic_cast<AacEncoder*>(enCdr);
   if (mp4Fi && aacEn) {
       aacEn->encoder(rawFr, deCdcCtx, mp4Fi, AU);
   }

   av_frame_free(&rawFr);
}
