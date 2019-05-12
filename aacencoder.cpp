#include "aacencoder.h"

AacEncoder::AacEncoder(QObject *parent) : Encoder(parent), mSwrCtx(nullptr), mAACFrame(nullptr),
    mExcedAACFr(nullptr), mAACPkt(nullptr), mSamplesCount(-1), mAACFifo(nullptr),
    mOutputFr(nullptr), mEnOnAacFrCom(nullptr) {

}

AacEncoder::~AacEncoder() {

}


void AacEncoder::setEnCtx() {
    mCdcCtx = avcodec_alloc_context3(mEnCdc);
    if (!mCdcCtx) {
        //Log::instance()->warn("AVCodecContext is null");
        //sendErr();
        //return;
        throw screenrecoder::AacEnException("AVCodecContext is null");
    }

    mCdcCtx->sample_fmt = mEnCdc->sample_fmts ? mEnCdc->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
    mCdcCtx->bit_rate = 64000;
    mCdcCtx->sample_rate = 44100;
    if (mEnCdc->supported_samplerates) {
        mCdcCtx->sample_rate = mEnCdc->supported_samplerates[0];
        for (int i = 0; mEnCdc->supported_samplerates[i]; i++) {
            if (44100 == mEnCdc->supported_samplerates[i]) {
                mCdcCtx->sample_rate = 44100;
            }
        }
    }
    mCdcCtx->channels = av_get_channel_layout_nb_channels(mCdcCtx->channel_layout);
    mCdcCtx->channel_layout = AV_CH_LAYOUT_STEREO;
    if (mEnCdc->channel_layouts) {
        mCdcCtx->channel_layout = mEnCdc->channel_layouts[0];
        for (int i = 0; mEnCdc->channel_layouts[i]; i++) {
            if (AV_CH_LAYOUT_STEREO == mEnCdc->channel_layouts[i]) {
                mCdcCtx->channel_layout = AV_CH_LAYOUT_STEREO;
            }
        }
    }
    mCdcCtx->channels = av_get_channel_layout_nb_channels(mCdcCtx->channel_layout);
    mCdcCtx->time_base.num = 1;
    mCdcCtx->time_base.den = mCdcCtx->sample_rate;

    mCdcCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
}

void AacEncoder::findEncoder() {
    mEnCdc = avcodec_find_encoder(AV_CODEC_ID_AAC);
    if (!mEnCdc) {
        //Log::instance()->warn("Can't find encoder");
        //sendErr();
        //return;
        throw screenrecoder::AacEnException("Can't find encoder");
    }
}

void AacEncoder::initEnCtx() {
    int ret = avcodec_open2(mCdcCtx, mEnCdc, nullptr);
    if (0 > ret) {
        //Log::instance()->warn(QString("Failed in avcodec_open2 func error code: %1").arg(ret));
        //sendErr();
        //return;
        throw screenrecoder::AacEnException(QString("Failed in avcodec_open2 func error code: %1").arg(ret));
    }
}

void AacEncoder::initConFrame() {
    mAACFrame = av_frame_alloc();
    if (!mAACFrame) {
        //Log::instance()->warn("Error allocating an audio frame \n");
        //sendErr();
        //return;
        throw screenrecoder::AacEnException("Error allocating an audio frame \n");
    }

    //int nb_samples = -1;
    //if (mEnCdc->capabilities & AV_CODEC_CAP_VARIABLE_FRAME_SIZE) {
    //    nb_samples = 10000;
    //}
    //else {
    //    nb_samples = mCdcCtx->frame_size;
    //}

    //mAACFrame->format = mCdcCtx->sample_fmt;
    //mAACFrame->channel_layout = mCdcCtx->channel_layout;
    //mAACFrame->sample_rate = mCdcCtx->sample_rate;
    //mAACFrame->nb_samples = nb_samples;

    //if (nb_samples) {
    //    if (0 > av_frame_get_buffer(mAACFrame, 0)) {
    //        Log::instance()->warn("Error allocating an audio buffer \n");
    //        sendErr();
    //    }
    //}
}

void AacEncoder::sendErr() {
    emit errorAACEn();
}


int64_t AacEncoder::getNextPts() {
    return mSamplesCount;
}

bool AacEncoder::getOnAacFrDaCom() {
    return *mEnOnAacFrCom ;
}

void AacEncoder::getOnAacFrDaUnCom() {
    *mEnOnAacFrCom = false;
}

void AacEncoder::excInvDa(uint relSamCo) {
    mExcedAACFr = av_frame_alloc();
    if (!mExcedAACFr) {
        //Log::instance()->warn("Error allocating an audio frame \n");
        //sendErr();
        //return;
        throw screenrecoder::AacEnException("Error allocating an audio frame \n");
    }

    mExcedAACFr->format = mCdcCtx->sample_fmt;
    mExcedAACFr->nb_samples = relSamCo;
    mExcedAACFr->sample_rate = mCdcCtx->sample_rate;
    mExcedAACFr->channel_layout = mCdcCtx->channel_layout;

    if (relSamCo) {
        if (0 > av_frame_get_buffer(mExcedAACFr, 0)) {
            //Log::instance()->warn("Error allocating an audio buffer \n");
            //sendErr();
            //return;
            throw screenrecoder::AacEnException("Error allocating an audio buffer \n");
        }
    }

    int dataSize = av_get_bytes_per_sample(mCdcCtx->sample_fmt);
    if (0 > dataSize) {
        //Log::instance()->warn("Failed to calculate pcm data size\n");
        //sendErr();
        //return;
        throw screenrecoder::AacEnException("Failed to calculate pcm data size\n");
    }
    if (av_sample_fmt_is_planar(mCdcCtx->sample_fmt)) {
        // 采样格式为平面
        for (int i = 0; i < relSamCo; i++) {
            for (int ch = 0; ch < mAACFrame->channels; ch++) {
                memcpy(mExcedAACFr->data[ch] + dataSize * i,
                         mAACFrame->data[ch] + dataSize * i, dataSize);
                //memcpy_s(mExcedAACFr->data[ch] + dataSize * i, dataSize,
                //         mAACFrame->data[ch] + dataSize * i, dataSize);
            }
        }
    } else {
        memcpy(mExcedAACFr->data[0], mAACFrame->data[0], relSamCo * dataSize);
        //memcpy_s(mExcedAACFr->data[0], relSamCo * dataSize,
        //                mAACFrame->data[0], relSamCo * dataSize);
    }
}

void AacEncoder::init(GenerateFile *mp4File) {
    findEncoder();
    setEnCtx();
    initEnCtx();
    initConFrame();
    initFifo();

    if (!mInitStCdcCtx)	{
        mp4File->initStCdcCtx(mCdcCtx, AU);
        mInitStCdcCtx = true;
    }

    mEnOnAacFrCom = new bool;
    *mEnOnAacFrCom = false;
}

void AacEncoder::initFifo() {
    mAACFifo = av_audio_fifo_alloc(mCdcCtx->sample_fmt, mCdcCtx->channels, 1);
    if (!mAACFifo) {
        //Log::instance()->warn("Could not allocate FIFO\n");
        //sendErr();
        //return;
        throw screenrecoder::AacEnException("Could not allocate FIFO\n");
    }
}

void AacEncoder::addSamToFifo(AVFrame *rawFr) {
    if (0 > av_audio_fifo_realloc(mAACFifo, av_audio_fifo_size(mAACFifo) + rawFr->nb_samples)) {
        //if (0 > av_audio_fifo_realloc(mAACFifo, av_audio_fifo_size(mAACFifo) + mExcedAACFr->nb_samples)) {
        //Log::instance()->warn("Could not reallocate FIFO\n");
        //sendErr();
        //return;
        throw screenrecoder::AacEnException("Could not reallocate FIFO\n");
    }

    if (rawFr->nb_samples > av_audio_fifo_write(mAACFifo, reinterpret_cast<void**>(rawFr->data), rawFr->nb_samples)) {
        //if (mExcedAACFr->nb_samples > av_audio_fifo_write(mAACFifo, reinterpret_cast<void**>(mExcedAACFr->data), mExcedAACFr->nb_samples)) {
        //Log::instance()->warn("Could not write data to FIFO\n");
        //sendErr();
        //return;
        throw screenrecoder::AacEnException("Could not write data to FIFO\n");
    }
}

void AacEncoder::initOpFr(uint frSi) {
    mOutputFr = av_frame_alloc();
    if (!mOutputFr) {
        //Log::instance()->warn("Error allocating an audio frame \n");
        //sendErr();
        //return;
        throw screenrecoder::AacEnException("Error allocating an audio frame \n");
    }

    mOutputFr->nb_samples = frSi;
    mOutputFr->channel_layout = mCdcCtx->channel_layout;
    mOutputFr->format = mCdcCtx->sample_fmt;
    mOutputFr->sample_rate = mCdcCtx->sample_rate;

    if (0 > av_frame_get_buffer(mOutputFr, 0)) {
        //Log::instance()->warn("Error allocating an audio buffer \n");
        //sendErr();
        //return;
        throw screenrecoder::AacEnException("Error allocating an audio buffer \n");
    }
}

void AacEncoder::initOpFrEx(AVFrame **outFr, uint frSi) {
    *outFr = av_frame_alloc();
    if (!(*outFr)) {
        //Log::instance()->warn("Error allocating an audio frame \n");
        //sendErr();
        //return;
        throw screenrecoder::AacEnException("Error allocating an audio frame \n");
    }

    (*outFr)->nb_samples = frSi;
    (*outFr)->channel_layout = mCdcCtx->channel_layout;
    (*outFr)->format = mCdcCtx->sample_fmt;
    (*outFr)->sample_rate = mCdcCtx->sample_rate;

    if (0 > av_frame_get_buffer(*outFr, 0)) {
        //Log::instance()->warn("Error allocating an audio buffer \n");
        //sendErr();
        //return;
        throw screenrecoder::AacEnException("Error allocating an audio buffer \n");
    }
}

void AacEncoder::encoder(AVFrame *srcFrame, const AVCodecContext *deCodecCtx, GenerateFile *mp4File, StT stt, bool fluFr) {
    if (!mSwrCtx) {
        mSwrCtx = swr_alloc();
        if (!mSwrCtx) {
            //Log::instance()->warn("Could not allocate resampler context\n");
            //sendErr();
            //return;
            throw screenrecoder::AacEnException("Could not allocate resampler context\n");
        }

        av_opt_set_int(mSwrCtx, "in_channel_count", deCodecCtx->channels, 0);
        av_opt_set_int(mSwrCtx, "in_sample_rate", deCodecCtx->sample_rate, 0);
        av_opt_set_int(mSwrCtx, "in_sample_fmt", deCodecCtx->sample_fmt, 0);
        av_opt_set_int(mSwrCtx, "out_channel_count", mCdcCtx->channels, 0);
        av_opt_set_int(mSwrCtx, "out_sample_rate", mCdcCtx->sample_rate, 0);
        av_opt_set_int(mSwrCtx, "out_sample_fmt", mCdcCtx->sample_fmt, 0);

        if (0 > swr_init(mSwrCtx)) {
            //Log::instance()->warn("Failed to initialize the resampler context\n");
            //sendErr();
            //return;
            throw screenrecoder::AacEnException("Failed to initialize the resampler context\n");
        }

        mAACPkt = av_packet_alloc();

        mMaxDstNbSam = mDstNbSamples = av_rescale_rnd(srcFrame->nb_samples,
                                                      mCdcCtx->sample_rate, deCodecCtx->sample_rate, AV_ROUND_UP);
        mAACFrame->format = mCdcCtx->sample_fmt;
        mAACFrame->channel_layout = mCdcCtx->channel_layout;
        mAACFrame->sample_rate = mCdcCtx->sample_rate;
        mAACFrame->nb_samples = mDstNbSamples;

        if (mDstNbSamples) {
            if (0 > av_frame_get_buffer(mAACFrame, 0)) {
                //Log::instance()->warn("Error allocating an audio buffer \n");
                //sendErr();
                //return;
                throw screenrecoder::AacEnException("Error allocating an audio buffer \n");
            }
        }
    }


    if (!fluFr) {
        mDstNbSamples = av_rescale_rnd(swr_get_delay(mSwrCtx, deCodecCtx->sample_rate) + srcFrame->nb_samples,
                                       mCdcCtx->sample_rate, deCodecCtx->sample_rate, AV_ROUND_UP);
        if (mMaxDstNbSam < mDstNbSamples) {
            av_frame_free(&mAACFrame);

            mAACFrame = av_frame_alloc();
            if (!mAACFrame) {
                //Log::instance()->warn("Error allocating an audio frame \n");
                //sendErr();
                //return;
                throw screenrecoder::AacEnException("Error allocating an audio frame \n");
            }
            mAACFrame->format = mCdcCtx->sample_fmt;
            mAACFrame->channel_layout = mCdcCtx->channel_layout;
            mAACFrame->sample_rate = mCdcCtx->sample_rate;
            mAACFrame->nb_samples = mDstNbSamples;

            if (mDstNbSamples) {
                if (0 > av_frame_get_buffer(mAACFrame, 0)) {
                    //Log::instance()->warn("Error allocating an audio buffer \n");
                    //sendErr();
                    //return;
                    throw screenrecoder::AacEnException("Error allocating an audio buffer \n");
                }
            }

            mMaxDstNbSam = mDstNbSamples;
        }
        //qDebug() << "mDstNbSamples: " << mDstNbSamples;
        // 以下代码是在不需要重采样的时候用的
        //if (mDstNbSamples != srcFrame->nb_samples) {
        //    Log::instance()->warn("Error !!!!");
        //    sendErr();
        //}

        if (0 > av_frame_make_writable(mAACFrame)) {
            //sendErr();
            //return;
            throw screenrecoder::AacEnException("Audio frame can not writable");
        }

        if ((mCdcCtx->sample_rate != deCodecCtx->sample_rate) || (mCdcCtx->channels != deCodecCtx->channels)
                || (mCdcCtx->sample_fmt != deCodecCtx->sample_fmt)) {
            int	relSamCo = swr_convert(mSwrCtx, mAACFrame->data, mDstNbSamples, const_cast<const uint8_t**>(srcFrame->data), srcFrame->nb_samples);
            qDebug() << "swr_convert return value: " << relSamCo;
            if (0 > relSamCo) {
                //Log::instance()->warn("Error while converting\n");
                //sendErr();
                //return;
                throw screenrecoder::AacEnException("Error while converting\n");
            }

            if (relSamCo != mDstNbSamples) {
                excInvDa(relSamCo);
                addSamToFifo(mExcedAACFr);
            } else {
                addSamToFifo(mAACFrame);
            }

        } else {
            addSamToFifo(srcFrame);
        }

        //if (0 > swr_convert(mSwrCtx, mAACFrame->data, mAACFrame->nb_samples, const_cast<const uint8_t**>(srcFrame->data), srcFrame->nb_samples)) {


        if (mCdcCtx->frame_size <= av_audio_fifo_size(mAACFifo)) {
            const int frSi = FFMIN(av_audio_fifo_size(mAACFifo), mCdcCtx->frame_size);

            initOpFr(frSi);

            if (frSi > av_audio_fifo_read(mAACFifo, reinterpret_cast<void**>(mOutputFr->data), frSi)) {
            //    //Log::instance()->warn("Could not read data from FIFO\n");
            //    //sendErr();
            //    //return;
                throw screenrecoder::AacEnException("Could not read data from FIFO\n");
            }

            AVRational tb;
            tb.num = 1;
            tb.den = mCdcCtx->sample_rate;
            //mOutputFr->pts = mSamplesCount;
            mOutputFr->pts = av_rescale_q(mSamplesCount, tb, mCdcCtx->time_base);
            //qDebug() << "deCodecCtx tb: " << "num: " << deCodecCtx->time_base.num << "den: " << deCodecCtx->time_base.den;
            mSamplesCount += mOutputFr->nb_samples;

            //mp4File->generatePcmFi(mCdcCtx->sample_fmt, mExcedAACFr, -1);

            int rlt = avcodec_send_frame(mCdcCtx, mOutputFr);
            if (0 > rlt) {
                //Log::instance()->warn("Error send frame to encoder\n");
                //sendErr();
                //return;
                throw screenrecoder::AacEnException(QString("Error send audio frame to encoder! "
                                                            "Error code: %1; Error string: %2\n").arg(rlt).
                                                            arg(PublicBuffer::instance()->getErrStr(rlt)));
            }

            while (0 <= avcodec_receive_packet(mCdcCtx, mAACPkt)) {
                mp4File->generateFile(mAACPkt, &mCdcCtx->time_base, stt);
            }

            av_frame_free(&mOutputFr);

            *mEnOnAacFrCom = true;
        }

        if (mExcedAACFr) {
            av_frame_free(&mExcedAACFr);
            mExcedAACFr = nullptr;
        }
    } else {
        if (0 > avcodec_send_frame(mCdcCtx, nullptr)) {
            //Log::instance()->warn("Error send frame to encoder\n");
            //sendErr();
            //return;
            throw screenrecoder::AacEnException("Error send frame to encoder\n");
        }

        while (0 <= avcodec_receive_packet(mCdcCtx, mAACPkt)) {
            mp4File->generateFile(mAACPkt, &mCdcCtx->time_base, stt);
        }
    }
}

void AacEncoder::clean() {
    av_frame_free(&mAACFrame);
    av_packet_free(&mAACPkt);
    av_audio_fifo_free(mAACFifo);
    avcodec_free_context(&mCdcCtx);
    swr_free(&mSwrCtx);

    mInitStCdcCtx = false;
    //*mEnOnAacFrCom= false;
    mCdcCtx = nullptr;
    mAACFrame = nullptr;
    mAACPkt = nullptr;
    mSwrCtx = nullptr;
    mNextPts = 0;
    mSamplesCount = 0;
    mDstNbSamples = 0;
    mMaxDstNbSam = 0;
}
