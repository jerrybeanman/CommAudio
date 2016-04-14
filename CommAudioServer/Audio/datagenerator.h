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

    void start();
    void stop();
    void resume();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Read Data
    --
    -- DATE:		March 9th, 2016             REVISIONS: March 19th, 2016 (Tyler Trepanier)
    --                                              -Added in network functionality.
    --
    -- DESIGNER:	Tyler Trepanier				PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	qint64 readData(char *data, qint64 maxlen);
    --
    -- PARAMETERS:          char *data
    --                          Data that will have the song copied onto it.
    --                      qint64 maxlen
    --                          Requested length of data to be read in however it is possible to read less data.
    --
    -- RETURNS: Number of characters read into the data.
    --
    -- NOTES: Opens the DataGenerator to allow for music streaming.
    --------------------------------------------------------------------------------------------------------------------*/
    qint64 readData(char *data, qint64 maxlen);

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Write Data
    --
    -- DATE:		March 9th, 2016             REVISIONS:
    --
    -- DESIGNER:	Tyler Trepanier				PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	qint64 writeData(const char *data, qint64 len);
    --
    -- PARAMETERS:          const char *data
    --                          Unused, removed write functionality
    --                      qint64 maxlen
    --                          Unused, removed write functionality
    --
    -- RETURNS: Always returns 0, this is unused.
    --
    -- NOTES: Removes QT's ability to write data.
    --------------------------------------------------------------------------------------------------------------------*/
    qint64 writeData(const char *data, qint64 len);

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Bytes Available
    --
    -- DATE:		Febuary 28th, 2016			REVISIONS:
    --
    -- DESIGNER:	Tyler Trepanier				PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	qint64 bytesAvailable() const;
    --
    -- RETURNS:     Total bytes inside of the generator.
    --
    -- NOTES: QT uses this function to determine the maximum amount of data that is read possible in readData. This
    -- returns the number of maximum bytes available to be read.
    --------------------------------------------------------------------------------------------------------------------*/
    qint64 bytesAvailable() const;

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Get External Reference
    --
    -- DATE:		March 18th, 2016			REVISIONS:
    --
    -- DESIGNER:	Tyler Trepanier				PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	QByteArray* getExternalReference();
    --
    -- RETURNS:     A reference to a copy of the song data inside the song generator.
    --
    -- NOTES: Returns a reference to a copy of the song data. This is a copy and not the actual reference so the actual
    -- song data is not manipulated in any way possible.
    --------------------------------------------------------------------------------------------------------------------*/
    QByteArray* getExternalReference();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Is Playing
    --
    -- DATE:		March 18th, 2016             REVISIONS:
    --
    -- DESIGNER:	Tyler Trepanier				 PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	bool isPlaying();
    --
    -- RETURNS:     boolean playing
    --
    -- NOTES: Returns if the datagenerator is currently playing or not. Used to see if the network is lagging behind
    -- and silently restarts playing.
    --------------------------------------------------------------------------------------------------------------------*/
    bool isPlaying();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Remove Buffered Data
    --
    -- DATE:		April 3rd, 2016             REVISIONS:
    --
    -- DESIGNER:	Tyler Trepanier				PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	void RemoveBufferedData();
    --
    -- RETURNS:     void
    --
    -- NOTES: Removes all allocated song data and resets the song generator to empty.
    --------------------------------------------------------------------------------------------------------------------*/
    void RemoveBufferedData();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Add More Data To Buffer From File
    --
    -- DATE:		March 9th, 2016             REVISIONS:
    --
    -- DESIGNER:	Tyler Trepanier				PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	void AddMoreDataToBufferFromFile(QFile* file, qint64 len);
    --
    -- PARAMETERS:      QFile* file
    --                      File containing the song data.
    --                  qint64 len
    --                      The desired size to read from the file.
    --
    -- RETURNS:     void
    --
    -- NOTES:       Adds in song data from the specified file.
    --------------------------------------------------------------------------------------------------------------------*/
    void AddMoreDataToBufferFromFile(QFile* file, qint64 len);

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Add More Data To Buffer From QByteArray
    --
    -- DATE:		March 9th, 2016             REVISIONS:
    --
    -- DESIGNER:	Tyler Trepanier				PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	void AddMoreDataToBufferFromQByteArray(QByteArray array, qint64 len);
    --
    -- PARAMETERS:      QByteArray array
    --                      File containing the song data.
    --                  qint64 len
    --                      The desired size to read from the QByteArray.
    --
    -- RETURNS:     void
    --
    -- NOTES:       Add in a portion
    --------------------------------------------------------------------------------------------------------------------*/
    void AddMoreDataToBufferFromQByteArray(QByteArray array, qint64 len);
    void resetPosition();
    QAudioFormat readHeader(char* data);
    void setValid();

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
