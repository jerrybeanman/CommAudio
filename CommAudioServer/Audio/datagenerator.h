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
    DataGenerator(QObject *parent);
    ~DataGenerator();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Start
    --
    -- DATE:		March 9th, 2016             REVISIONS:
    --
    -- DESIGNER:	Tyler Trepanier				PROGRAMMER: Tyler Trepanier
    --
    -- INTERFACE:	void start();
    --
    -- RETURNS: void
    --
    -- NOTES: Opens the DataGenerator to allow for music streaming.
    --------------------------------------------------------------------------------------------------------------------*/
    void start();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Stop
    --
    -- DATE:		March 9th, 2016             REVISIONS:
    --
    -- DESIGNER:	Tyler Trepanier				PROGRAMMER:	Ruoqi Jia
    --
    -- INTERFACE:	void stop();
    --
    -- RETURNS: void
    --
    -- NOTES: Stops the DataGenerator from streaming music
    --------------------------------------------------------------------------------------------------------------------*/
    void stop();


    qint64 readData(char *data, qint64 maxlen);
    qint64 readExternalData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
    qint64 bytesAvailable() const;
    QByteArray* getExternalReference();
    bool isPlaying();
    void RemoveBufferedData();
    void AddMoreDataToBufferFromFile(QFile* file, qint64 len);
    void AddMoreDataToBufferFromQByteArray(QByteArray array, qint64 len);
    void setValid();
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
    qint64      dg_max;
    QByteArray  dg_buffer;
    QByteArray  dg_externBuf;
    bool        playing;
    bool        validFormat;
};
#endif // DATAGENERATOR_H
