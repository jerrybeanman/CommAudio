#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <QAudioFormat>
#include <QAudioDecoder>
#include <QDebug>
#include <QFileDialog>
#include <QBuffer>
#include <QAudioOutput>
#include "wavfile.h"

class DataGenerator : public QIODevice
{
    Q_OBJECT

public:
    DataGenerator(QObject *parent);
    ~DataGenerator();

    void start();
    void stop();
    void resume();

    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
    qint64 bytesAvailable() const;
    bool isPlaying();
    void RemoveBufferedData();
    void AddMoreDataToBufferFromFile(QFile* file, qint64 len);
    void AddMoreDataToBufferFromQByteArray(QByteArray array, qint64 len);
    void resetPosition();

signals:
    void audioProgressChanged(int progress);

public:
    float progress;

private:
    qint64      dg_pos;
    qint64      dg_max;
    QByteArray  dg_buffer;
    bool        playing;
};
#endif // DATAGENERATOR_H
