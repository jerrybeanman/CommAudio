#-------------------------------------------------
#
# Project created by QtCreator 2016-02-25T23:09:20
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CommAudio
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
    circularbuffer.cpp \
    Client/ClientUDP.cpp \
    wavfile.cpp \
    datagenerator.cpp \
    recorder.cpp \
    inputbuffer.cpp \
    threadmanager.cpp

HEADERS  += mainwindow.h \
    circularbuffer.h \
    globals.h \
    Client/Client.h \
    Client/ClientUDP.h \
    wavfile.h \
    datagenerator.h \
    recorder.h \
    inputbuffer.h \
    threadmanager.h

FORMS    += mainwindow.ui
