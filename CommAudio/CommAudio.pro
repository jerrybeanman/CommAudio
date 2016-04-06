#-------------------------------------------------
#
# Project created by QtCreator 2016-02-25T23:09:20
#
#-------------------------------------------------

QT       += core gui

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

SOURCES += \
    Client/ClientUDP.cpp \
    Client/ClientTCP.cpp \
    circularbuffer.cpp \
    main.cpp \
    mainwindow.cpp \
    tcpthreadmanager.cpp \
    udpthreadmanager.cpp \
    Audio/datagenerator.cpp \
    Audio/inputbuffer.cpp \
    Audio/recorder.cpp \
    Audio/wavfile.cpp

HEADERS  += \
    Client/client.h \
    Client/ClientTCP.h \
    circularbuffer.h \
    globals.h \
    mainwindow.h \
    Client/ClientUDP.h \
    tcpthreadmanager.h \
    udpthreadmanager.h \
    Audio/datagenerator.h \
    Audio/inputbuffer.h \
    Audio/recorder.h \
    Audio/wavfile.h

FORMS    += mainwindow.ui
