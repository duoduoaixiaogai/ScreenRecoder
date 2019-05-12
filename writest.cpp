#include "writest.h"

WriteSt::WriteSt(QObject *parent) : SRException(parent),
    mDeVdCtx(nullptr), mDeAuCtx(nullptr), mStoWr(nullptr), mStaingWr(nullptr),
    mInited(nullptr), mEnVd(nullptr), mEnAu(nullptr), mAlWrHe(nullptr), mAlWrTr(nullptr),
    mImStoWr(nullptr), mImStoWrComed(nullptr), mStoWrCom(nullptr) {

}

WriteSt::~WriteSt() {

}

void WriteSt::startingWr(const bool enableVd, const bool enableAu) {
}

void WriteSt::pausingWr() {
    if (mStoWrCom) {
        *mStoWrCom = false;
    }
    uninit();
}

void WriteSt::imStoWr() {
    if (mImStoWr) {
        *mImStoWr = true;
    }
}

bool WriteSt::alStaWr() {
   if (mStaingWr) {
       return *mStaingWr;
   }
   return false;
}

bool WriteSt::stoWrCom() {
   if (mStoWrCom) {
       return *mStoWrCom;
   }
   return true;
}

void WriteSt::wringFiTra() {

}

bool WriteSt::imStoWrComed() {
    if (mImStoWrComed) {
        return *mImStoWrComed;
    }
    return true;
}

void WriteSt::clean() {

}

void WriteSt::flush() {}

void WriteSt::init(const bool enableVd, const bool enableAu) {

}

void WriteSt::uninit() {

}
