#ifndef RECORDER_H
#define RECORDER_H

#include <QApplication>
#include <QAudioInput>
#include <QBuffer>
#include <QDataStream>
#include <QDebug>

class Recorder : public QObject
{
    Q_OBJECT
public:
    Recorder();
    ~Recorder();
    const QAudioFormat &fileFormat() const;
    void start();
    void stop();
    QByteArray readAll() const;
    int bytesWritten();
    void swapBuffers();

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
    bool                bufferFull;
    QIODevice           *dev;
    QBuffer             *r_newBuffer;
    QBuffer             *r_saveBuffer;
    QByteArray          r_byteArray;
    QByteArray          r_readyArray;
    bool                inProgress;
    bool                ready;
    int                 audio_state;
    int                 writeBuffer;


signals:

};

#endif // RECORDER_H
