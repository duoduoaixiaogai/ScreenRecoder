#include "publicbuffer.h"

PublicBuffer* PublicBuffer::mInstance = nullptr;

PublicBuffer::PublicBuffer(QObject *parent) : QObject(parent), mRawVCdcCtx(nullptr),
    mRawACdcCtx(nullptr), mMaxBufMemsCou(1), mTimeout(1000) {

}

PublicBuffer::~PublicBuffer() {
    if (mInstance) {
        delete mInstance;
        mInstance = nullptr;
    }
}

PublicBuffer* PublicBuffer::instance() {
    if (!mInstance) {
        mInstance = new PublicBuffer;
    }
    return mInstance;
}

void PublicBuffer::putRawVFr(AVFrame *rawFr) {
    mMutex.lock();
    if (mMaxBufMemsCou == mRawVFrsBuf.count()) {
        mRawVFrsBufNotFull.wait(&mMutex, mTimeout);
    }
    mMutex.unlock();

    mMutex.lock();
    mRawVFrsBuf.enqueue(rawFr);
    mRawVFrsBufNotEmpty.wakeOne();
    mMutex.unlock();
}

void PublicBuffer::putRawAFr(AVFrame *rawFr) {
    mMutex.lock();
    if (mMaxBufMemsCou == mRawAFrsBuf.count()) {
        mRawAFrsBufNotFull.wait(&mMutex, mTimeout);
    }
    mMutex.unlock();

    mMutex.lock();
    mRawAFrsBuf.enqueue(rawFr);
    mRawAFrsBufNotEmpty.wakeOne();
    mMutex.unlock();
}

AVFrame* PublicBuffer::getRawVFr() {
    mMutex.lock();
    if (0 == mRawVFrsBuf.count()) {
        mRawVFrsBufNotEmpty.wait(&mMutex, mTimeout);
    }
    mMutex.unlock();

    mMutex.lock();
    AVFrame *rawVFr = nullptr;
    if (!mRawVFrsBuf.isEmpty()) {
        rawVFr = mRawVFrsBuf.dequeue();
    }
    mRawVFrsBufNotFull.wakeOne();
    mMutex.unlock();

    return rawVFr;
}

AVFrame* PublicBuffer::getRawAFr() {
    mMutex.lock();
    if (0 == mRawAFrsBuf.count()) {
        mRawAFrsBufNotEmpty.wait(&mMutex, mTimeout);
    }
    mMutex.unlock();

    mMutex.lock();
    AVFrame *rawAFr = nullptr;
    if (!mRawAFrsBuf.isEmpty()) {
        rawAFr = mRawAFrsBuf.dequeue();
    }
    mRawAFrsBufNotFull.wakeOne();
    mMutex.unlock();

    return rawAFr;
}

bool PublicBuffer::isRawVFrsBufEmpty() {
    return mRawVFrsBuf.isEmpty();
}

bool PublicBuffer::isRawAFrsBufEmpty() {
    return mRawAFrsBuf.isEmpty();
}

int PublicBuffer::getRawVFrsBufMemCou() {
    return mRawVFrsBuf.count();
}

int PublicBuffer::getRawAFrsBufMemCou() {
    return mRawAFrsBuf.count();
}

void PublicBuffer::setRawVCdcCtx(const AVCodecContext *cdcCtx) {
    mRawVCdcCtx = cdcCtx;
}

void PublicBuffer::setRawACdcCtx(const AVCodecContext *cdcCtx) {
    mRawACdcCtx = cdcCtx;
}

const AVCodecContext* PublicBuffer::getRawVCdcCtx() const {
    return mRawVCdcCtx;
}

const AVCodecContext* PublicBuffer::getRawACdcCtx() const {
    return mRawACdcCtx;
}

void PublicBuffer::wakeWrVTh() {
    mRawVFrsBufNotEmpty.wakeAll();
    //mRawVFrsBufNotFull.wakeOne();
}

void PublicBuffer::wakeWrATh() {
    mRawAFrsBufNotEmpty.wakeAll();
}

QString PublicBuffer::getErrStr(int errNum) {
    char errBuf[512];
    memset(errBuf, 0, 512);
    av_strerror(errNum, errBuf, 512);

    return QString(errBuf);
}

void PublicBuffer::clean() {
    mRawVCdcCtx = nullptr;
    mRawACdcCtx = nullptr;
    if (!mRawVFrsBuf.isEmpty()) {
        mRawVFrsBuf.clear();
    }
    if (mRawAFrsBuf.isEmpty()) {
        mRawAFrsBuf.clear();
    }
}
