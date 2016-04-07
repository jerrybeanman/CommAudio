#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <QAudioFormat>
#include <QAudioDecoder>
#include <QDebug>
#include <QFileDialog>
#include <QBuffer>
#include <QAudioOutput>
#include <qendian.h>
#include <QVector>
#include <QDebug>
#include "wavfile.h"

class DataGenerator : public QIODevice
{
    Q_OBJECT

public:
    DataGenerator();
    ~DataGenerator();

    void start();
    void stop();
    void resume();

    qint64 readData(char *data, qint64 maxlen);
    qint64 readExternalData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
    qint64 bytesAvailable() const;
    QByteArray* getExternalReference();
    bool isPlaying();
    void RemoveBufferedData();
    void AddMoreDataToBufferFromFile(QFile* file, qint64 len);
    void AddMoreDataToBufferFromQByteArray(QByteArray array, qint64 len);
    void resetPosition();
    QAudioFormat readHeader(char* data);

signals:
    void audioProgressChanged(int progress);
    void dataAvailable(int len);
    void dataFinished();

public:
    float progress;
    int externChunk;


private:
    qint64      dg_readpos;
    qint64      dg_streampos;
    qint64      dg_max;
    QByteArray  dg_buffer;
    QByteArray  dg_externBuf;
    bool        playing;
};
#endif // DATAGENERATOR_H
