#ifndef RECORDER_H
#define RECORDER_H

#include <QApplication>
#include <QAudioInput>
#include <QBuffer>
#include <QDataStream>

class Recorder : public QObject
{
    Q_OBJECT
public:
    Recorder();
    ~Recorder();
    const QAudioFormat &fileFormat() const;
    void start();
    QByteArray readAll() const;
    int bytesWritten();

private:
    bool SetFormat();

private:
    QAudioFormat    r_format;
    QAudioInput     *r_input;
    bool            bufferFull;
    QIODevice       *dev;
    QBuffer         *r_buffer;
    QByteArray      r_byteArray;
    QByteArray      r_readyArray;
    bool            inProgress;
    bool            ready;


signals:

public slots:
    void dataReady();
};

#endif // RECORDER_H
