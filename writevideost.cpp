#include "writevideost.h"

WriteVideoSt::WriteVideoSt(QObject *parent) : WriteStEx(parent) {

}

WriteVideoSt::~WriteVideoSt() {

}

void WriteVideoSt::writeRawFr(AVFrame* rawFr, const AVCodecContext *deCdcCtx, Encoder *enCdr, GenerateFile *fi) {
    H264Encoder *h264En = dynamic_cast<H264Encoder*>(enCdr);
    Mp4File *mp4Fi = dynamic_cast<Mp4File*>(fi);
    if (h264En && mp4Fi) {
        h264En->encoder(rawFr, deCdcCtx, mp4Fi, VD);
    }

    av_frame_free(&rawFr);
}
