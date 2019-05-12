#-------------------------------------------------
#
# Project created by QtCreator 2018-11-29T09:49:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ScrrenRecoder
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#CONFIG += x86
CONFIG += x64

SOURCES += main.cpp\
        mainwindow.cpp \
    decoder.cpp \
    encoder.cpp \
    generatefile.cpp \
    aacencoder.cpp \
    log.cpp \
    h264encoder.cpp \
    mp4file.cpp \
    srexception.cpp \
    controlthreadex.cpp \
    readvideost.cpp \
    readst.cpp \
    readaudiost.cpp \
    wrh264aacst.cpp \
    writest.cpp \
    publicbuffer.cpp

HEADERS  += mainwindow.h \
    decoder.h \
    encoder.h \
    h264encoder.h \
    generatefile.h \
    mp4file.h \
    register.h \
    videodecoder.h \
    log.h \
    audiodecoder.h \
    aacencoder.h \
    srexception.h \
    controlthreadex.h \
    readvideost.h \
    readst.h \
    readaudiost.h \
    wrh264aacst.h \
    writest.h \
    publicbuffer.h

FORMS    += mainwindow.ui

FFMPEG_INCLUDE
FFMPEG_LIB
SPDLOG_INCLUDE

CONFIG(x64, x64 | x86) {
    win32 {
        FFMPEG_INCLUDE = $$PWD/x64/windows/ffmpeg/include
        FFMPEG_LIB = $$PWD/x64/windows/ffmpeg/lib
        SPDLOG_INCLUDE = $$PWD/x64/windows/spdlog/include
    }
    unix {
        FFMPEG_INCLUDE = $$PWD/x64/linux/ffmpeg/include
        FFMPEG_LIB = $$PWD/x64/linux/ffmpeg/lib
        SPDLOG_INCLUDE = $$PWD/x64/linux/spdlog/include
    }
} else {
    win32 {
        FFMPEG_INCLUDE = $$PWD/x86/windows/ffmpeg/include
        FFMPEG_LIB = $$PWD/x86/windows/ffmpeg/lib
        SPDLOG_INCLUDE = $$PWD/x86/windows/spdlog/include
    }
    unix {
        FFMPEG_INCLUDE = $$PWD/x86/linux/ffmpeg/include
        FFMPEG_LIB = $$PWD/x86/linux/ffmpeg/lib
        SPDLOG_INCLUDE = $$PWD/x86/linux/spdlog/include
    }
}
INCLUDEPATH += $$FFMPEG_INCLUDE \
        $$SPDLOG_INCLUDE


LIBS += -L$$FFMPEG_LIB \
        -lavcodec \
        -lavdevice \
        -lavfilter \
        -lavformat \
        -lavutil \
        -lpostproc \
        -lswresample \
        -lswscale

DISTFILES += \
    config \
    config.xml


TRANSLATIONS += ScreenRecoder_zh.ts
