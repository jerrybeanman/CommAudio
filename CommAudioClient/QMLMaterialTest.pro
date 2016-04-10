TEMPLATE = app

QT += qml quick multimedia core gui widgets multimediawidgets network xml
CONFIG += c++11

SOURCES += main.cpp \
    windowadapter.cpp \
    circularbuffer.cpp \
    tcpthreadmanager.cpp \
    udpthreadmanager.cpp \
    Audio/datagenerator.cpp \
    Audio/inputbuffer.cpp \
    Audio/recorder.cpp \
    Audio/wavfile.cpp \
    Client/ClientTCP.cpp \
    Client/clientudp.cpp

RESOURCES += qml.qrc

LIBS += -lws2_32

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = ["modules"]

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    matdesign.qml

HEADERS += \
    windowadapter.h \
    circularbuffer.h \
    globals.h \
    tcpthreadmanager.h \
    udpthreadmanager.h \
    Audio/datagenerator.h \
    Audio/inputbuffer.h \
    Audio/recorder.h \
    Audio/wavfile.h \
    Client/client.h \
    Client/ClientTCP.h \
    Client/clientudp.h
