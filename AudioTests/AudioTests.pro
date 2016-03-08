#-------------------------------------------------
#
# Project created by QtCreator 2016-03-02T16:43:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AudioTests
TEMPLATE = app

QT += network \
      xml \
      multimedia \
      multimediawidgets \
      widgets

SOURCES +=  main.cpp\
            AudioStream.cpp \
            player.cpp \
            playerscontrols.cpp \
            playlistmodel.cpp \
            generator.cpp \
    wavfile.cpp

HEADERS  += AudioStream.h \
            player.h \
            playerscontrols.h \
            playlistmodel.h \
            generator.h \
    wavfile.h

FORMS    += mainwindow.ui
