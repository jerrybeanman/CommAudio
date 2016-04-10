#ifndef RECORDER_H
#define RECORDER_H

#include <QApplication>
#include <QAudioInput>
#include <QBuffer>
#include <QDataStream>
#include <QDebug>
#include "inputbuffer.h"
#include "globals.h"

class Recorder : public QObject
{
    Q_OBJECT
public:
    Recorder();
    ~Recorder();
    const QAudioFormat &fileFormat() const;
    void start();
    void stop();
    const QByteArray readAll();
    int bytesWritten();

public slots:
    void notified();
    void handleAudioInputState(QAudio::State state);

private:
    bool SetFormat();
    void clearData();

private:
    QAudioFormat        r_format;
    QAudioInput         *r_input;
    QAudioDeviceInfo    r_inputInfo;
    QIODevice           *dev;
    InputBuffer         *r_newBuffer;
    InputBuffer         *r_saveBuffer;
    bool                inProgress;
    bool                ready;
    int                 audio_state;
    int                 r_bytes_AVAILABLE;
    int                 size;
    char*               r_data;
    qint64              pos;

signals:
    void dataAvailable(int len);

};

#endif // RECORDER_H
