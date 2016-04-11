#ifndef RECORDER_H
#define RECORDER_H

#include <QApplication>
#include <QAudioInput>
#include <QBuffer>
#include <QDataStream>
#include <QDebug>
#include "inputbuffer.h"
#include "globals.h"

class Recorder : public QIODevice
{
    Q_OBJECT
public:
    Recorder();
    ~Recorder();
    const QAudioFormat &fileFormat() const;
    void start();
    void stop();
    qint64 writeData(const char *data, qint64 len);
    qint64 readData(char * data, qint64 maxlen);
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
    QByteArray          r_buffer;
    bool                inProgress;
    bool                ready;
    int                 audio_state;
    int                 r_bytes_AVAILABLE;
    int                 size;
    char*               r_data;
signals:
    void dataAvailable(int len);

};

#endif // RECORDER_H
