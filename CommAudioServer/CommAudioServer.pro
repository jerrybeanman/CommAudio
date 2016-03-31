#-------------------------------------------------
#
# Project created by QtCreator 2016-03-07T21:46:33
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CommAudioServer
TEMPLATE = app

QT += network \
      xml \
      multimedia \
      multimediawidgets \
      widgets

LIBS += -lws2_32

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    Server/Server.cpp \
    Server/ServerTCP.cpp \
    Server/ServerUDP.cpp \
    circularbuffer.cpp \
    soundmanager.cpp \
    filemanager.cpp \
    datagenerator.cpp \
    inputbuffer.cpp \
    recorder.cpp \
    wavfile.cpp

HEADERS  += mainwindow.h \
    Server/server.h \
    Server/ServerTCP.h \
    Server/ServerUDP.h \
    circularbuffer.h \
    globals.h \
    soundmanager.h \
    filemanager.h \
    datagenerator.h \
    inputbuffer.h \
    recorder.h \
    wavfile.h

FORMS    += mainwindow.ui
CONFIG   += c++11
CONFIG   += debug
