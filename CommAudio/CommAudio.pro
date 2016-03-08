#-------------------------------------------------
#
# Project created by QtCreator 2016-02-25T23:09:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CommAudio
TEMPLATE = app

LIBS += -lws2_32

SOURCES += main.cpp\
        mainwindow.cpp \
    circularbuffer.cpp \
    Server/ServerTCP.cpp \
    Server/ServerUDP.cpp \
    Client/Client.cpp \
    Client/ClientUDP.cpp

HEADERS  += mainwindow.h \
    circularbuffer.h \
    globals.h \
    Server/ServerTCP.h \
    Server/Server.h \
    Server/ServerUDP.h \
    Client/Client.h \
    Client/ClientUDP.h

FORMS    += mainwindow.ui
