#-------------------------------------------------
#
# Project created by QtCreator 2016-03-07T21:46:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CommAudioServer
TEMPLATE = app

LIBS += -lws2_32

SOURCES += main.cpp\
        mainwindow.cpp \
    Server/Server.cpp \
    Server/ServerTCP.cpp \
    Server/ServerUDP.cpp \
    circularbuffer.cpp \
    soundmanager.cpp \
    filemanager.cpp

HEADERS  += mainwindow.h \
    Server/server.h \
    Server/ServerTCP.h \
    Server/ServerUDP.h \
    circularbuffer.h \
    globals.h \
    soundmanager.h \
    filemanager.h

FORMS    += mainwindow.ui
