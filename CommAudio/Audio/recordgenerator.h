#ifndef RECORDGENERATOR_H
#define RECORDGENERATOR_H

#include <QAudioFormat>
#include <QAudioDecoder>
#include <QDebug>
#include <QFileDialog>
#include <QBuffer>
#include <QAudioOutput>
#include <qendian.h>
#include <QVector>
#include <QDebug>

class RecordGenerator : public QIODevice
{
    Q_OBJECT
public:
    RecordGenerator();

    void start();
    void stop();
    void resume();
    bool isPlaying();
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
    void AddMoreDataToBufferFromQByteArray(QByteArray array, qint64 len);

private:
    void RemoveBufferedData(int len);

signals:

public:
    float progress;

private:
    qint64      rg_readpos;
    qint64      rg_max;
    QByteArray  rg_buffer;
    bool        playing;
};

#endif // RECORDGENERATOR_H
