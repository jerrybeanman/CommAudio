/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: datagenerator.h - Song Data Container for reading
--
-- PROGRAM:     commAudio
--
-- FUNCTIONS:
--              void start();
--              void stop();
--              void resume();
--              void RestartPlaying();
--              qint64 readData(char *data, qint64 maxlen);
--              qint64 writeData(const char *data, qint64 len);
--              qint64 bytesAvailable() const;
--              QByteArray* getExternalReference();
--              bool isPlaying();
--              void RemoveBufferedData();
--              void AddMoreDataToBufferFromFile(QFile* file, qint64 len);
--              void AddMoreDataToBufferFromQByteArray(QByteArray array, qint64 len);
--              void resetPosition();
--              QAudioFormat readHeader(char* data);
--              void audioProgressChanged(int progress);
--              void dataAvailable(int len);
--              void dataFinished();
--
-- DATE:        March 9th, 2016
--
-- REVISIONS:   (Date and Description)
--
-- DESIGNER:    Tyler Trepanier
--
-- PROGRAMMER:  Tyler Trepanier
--
-- NOTES:
-- This is the song data container which temporarily holds the data until it is released from memory when the next
-- song is played.
----------------------------------------------------------------------------------------------------------------------*/

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

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Start
    --
    -- DATE:        March 18th, 2016            REVISIONS:  April 9th, 2016 (Tyler Trepanier)
    --                                                          -Implemented network functionality
    --
    -- DESIGNER:    Tyler Trepanier             PROGRAMMER: Tyler Trepanier
    --
    -- INTERFACE:   void start();
    --
    -- RETURNS:     Opens the generator
    --
    -- NOTES: This prepares the Generator to being streaming from the input device.
    --------------------------------------------------------------------------------------------------------------------*/
    void start();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Stop
    --
    -- DATE:        March 18th, 2016            REVISIONS:
    --
    -- DESIGNER:    Tyler Trepanier             PROGRAMMER: Tyler Trepanier
    --
    -- INTERFACE:   void stop();
    --
    -- RETURNS: void
    --
    -- NOTES: Stops the Generator from reading
    --------------------------------------------------------------------------------------------------------------------*/
    void stop();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Resume
    --
    -- DATE:        March 18th, 2016            REVISIONS:
    --
    -- DESIGNER:    Tyler Trepanier             PROGRAMMER: Tyler Trepanier
    --
    -- INTERFACE:   void resume();
    --
    -- RETURNS: void
    --
    -- NOTES: Stops the Generator from reading
    --------------------------------------------------------------------------------------------------------------------*/
    void resume();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Restart Playing
    --
    -- DATE:        April 11th, 2016            REVISIONS:
    --
    -- DESIGNER:    Tyler Trepanier             PROGRAMMER: Tyler Trepanier
    --
    -- INTERFACE:   void RestartPlaying();
    --
    -- PARAMETERS:  void
    --
    -- RETURNS:     void
    --
    -- NOTES: Restarts the Data Generator before adding more data to the buffer.
    --------------------------------------------------------------------------------------------------------------------*/
    void RestartPlaying();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Read Data
    --
    -- DATE:        March 9th, 2016             REVISIONS: March 19th, 2016 (Tyler Trepanier)
    --                                              -Added in network functionality.
    --
    -- DESIGNER:    Tyler Trepanier             PROGRAMMER: Tyler Trepanier
    --
    -- INTERFACE:   qint64 readData(char *data, qint64 maxlen);
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
    -- FUNCTION:    Write Data
    --
    -- DATE:        March 9th, 2016             REVISIONS:
    --
    -- DESIGNER:    Tyler Trepanier             PROGRAMMER: Tyler Trepanier
    --
    -- INTERFACE:   qint64 writeData(const char *data, qint64 len);
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
    -- FUNCTION:    Bytes Available
    --
    -- DATE:        Febuary 28th, 2016          REVISIONS:
    --
    -- DESIGNER:    Tyler Trepanier             PROGRAMMER: Tyler Trepanier
    --
    -- INTERFACE:   qint64 bytesAvailable() const;
    --
    -- RETURNS:     Total bytes inside of the generator.
    --
    -- NOTES: QT uses this function to determine the maximum amount of data that is read possible in readData. This
    -- returns the number of maximum bytes available to be read.
    --------------------------------------------------------------------------------------------------------------------*/
    qint64 bytesAvailable() const;

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Get External Reference
    --
    -- DATE:        March 18th, 2016            REVISIONS:
    --
    -- DESIGNER:    Tyler Trepanier             PROGRAMMER: Tyler Trepanier
    --
    -- INTERFACE:   QByteArray* getExternalReference();
    --
    -- RETURNS:     A reference to a copy of the song data inside the song generator.
    --
    -- NOTES: Returns a reference to a copy of the song data. This is a copy and not the actual reference so the actual
    -- song data is not manipulated in any way possible.
    --------------------------------------------------------------------------------------------------------------------*/
    QByteArray* getExternalReference();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Is Playing
    --
    -- DATE:        March 18th, 2016             REVISIONS:
    --
    -- DESIGNER:    Tyler Trepanier              PROGRAMMER:    Tyler Trepanier
    --
    -- INTERFACE:   bool isPlaying();
    --
    -- RETURNS:     boolean playing
    --
    -- NOTES: Returns if the datagenerator is currently playing or not. Used to see if the network is lagging behind
    -- and silently restarts playing.
    --------------------------------------------------------------------------------------------------------------------*/
    bool isPlaying();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Remove Buffered Data
    --
    -- DATE:        April 3rd, 2016             REVISIONS:
    --
    -- DESIGNER:    Tyler Trepanier             PROGRAMMER: Tyler Trepanier
    --
    -- INTERFACE:   void RemoveBufferedData();
    --
    -- RETURNS:     void
    --
    -- NOTES: Removes all allocated song data and resets the song generator to empty.
    --------------------------------------------------------------------------------------------------------------------*/
    void RemoveBufferedData();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Add More Data To Buffer From File
    --
    -- DATE:        March 9th, 2016             REVISIONS:
    --
    -- DESIGNER:    Tyler Trepanier             PROGRAMMER: Tyler Trepanier
    --
    -- INTERFACE:   void AddMoreDataToBufferFromFile(QFile* file, qint64 len);
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
    -- FUNCTION:    Add More Data To Buffer From QByteArray
    --
    -- DATE:        March 9th, 2016             REVISIONS:
    --
    -- DESIGNER:    Tyler Trepanier             PROGRAMMER: Tyler Trepanier
    --
    -- INTERFACE:   void AddMoreDataToBufferFromQByteArray(QByteArray array, qint64 len);
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

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Reset Position
    --
    -- DATE:        April 9th, 2016             REVISIONS:
    --
    -- DESIGNER:    Tyler Trepanier             PROGRAMMER: Tyler Trepanier
    --
    -- INTERFACE:   void resetPosition();
    --
    -- RETURNS: void
    --
    -- NOTES: Resets the song position to the end of the stream.
    --------------------------------------------------------------------------------------------------------------------*/
    void resetPosition();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Read Header
    --
    -- DATE:        March 28th, 2016    REVISIONS:
    --
    -- DESIGNER:    Tyler Trepanier     PROGRAMMER: Tyler Trepanier
    --
    -- INTERFACE:   QAudioFormat readHeader(char* data);
    --
    -- PARAMETERS:          char * data
    --                          data that contains the potential song header info.
    --
    -- RETURNS:     The recommended song playback format.
    --
    -- NOTES: Reads the song header information from the data and returns a formatted audio playback format.
    --------------------------------------------------------------------------------------------------------------------*/
    QAudioFormat readHeader(char* data);

signals:
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Audio Progress Changed
    --
    -- DATE:        March 9th, 2016             REVISIONS:
    --
    -- DESIGNER:    Tyler Trepanier             PROGRAMMER: Tyler Trepanier
    --
    -- INTERFACE:   void audioProgressChanged(int progress);
    --
    -- RETURNS:     void
    --
    -- NOTES: Emits the current song progress to whatever QObject that catches it.
    --------------------------------------------------------------------------------------------------------------------*/
    void audioProgressChanged(int progress);

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Data Avaiable
    --
    -- DATE:        March 9th, 2016             REVISIONS:
    --
    -- DESIGNER:    Tyler Trepanier             PROGRAMMER: Tyler Trepanier
    --
    -- INTERFACE:   void dataAvailable(int len);
    --
    -- RETURNS:     void
    --
    -- NOTES: Emits a signal containing the current song data available to send.
    --------------------------------------------------------------------------------------------------------------------*/
    void dataAvailable(int len);

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Data Finished
    --
    -- DATE:        March 18th, 2016            REVISIONS:
    --
    -- DESIGNER:    Tyler Trepanier             PROGRAMMER: Tyler Trepanier
    --
    -- INTERFACE:   void dataFinished();
    --
    -- RETURNS:     void
    --
    -- NOTES: A song has finished playing and emits the signal.
    --------------------------------------------------------------------------------------------------------------------*/
    void dataFinished();

public:
    float progress;     // Current song progress.
    int externChunk;    // Size of the song data available to send.


private:
    qint64      dg_readpos;     // Current read position
    qint64      dg_max;         // Maximum buffer size
    QByteArray  dg_buffer;      // Primary song container
    QByteArray  dg_externBuf;   // Secondary song container used for transmission
    bool        playing;        // Flag that checks if the song generator is in use.
};
#endif // DATAGENERATOR_H
