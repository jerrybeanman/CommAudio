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

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Start
    --
    -- DATE:		April 9th, 2016             REVISIONS:
    --
    -- DESIGNER:	Tyler Trepanier				PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	void start();
    --
    -- RETURNS:     Opens the recorder and begins streaming from the default mic detected.
    --
    -- NOTES: This prepares the recorder to being streaming from the input device.
    --------------------------------------------------------------------------------------------------------------------*/
    void start();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Stop
    --
    -- DATE:		April 9th, 2016             REVISIONS:
    --
    -- DESIGNER:	Tyler Trepanier				PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	void stop();
    --
    -- RETURNS:     void
    --
    -- NOTES: Stops the Recorder from reading from the input device and deletes the input device.
    --------------------------------------------------------------------------------------------------------------------*/
    void stop();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Is Playing
    --
    -- DATE:		April 9th, 2016             REVISIONS:
    --
    -- DESIGNER:	Tyler Trepanier				PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	bool isPlaying();
    --
    -- RETURNS:     True if the record generator is being used, false otherwise.
    --
    -- NOTES: Returns the value of playing which signifies if this is playing voice data or not.
    --------------------------------------------------------------------------------------------------------------------*/
    bool isPlaying();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Write Data
    --
    -- DATE:		April 9th, 2016		REVISIONS:
    --
    -- DESIGNER:	Tyler Trepanier		PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	qint64 writeData(const char *data, qint64 len);
    --
    -- PARAMETERS:			const char *data
    --							Unused element.
    --						qint64 len
    --							Unused element.
    --
    -- RETURNS:     Returns 0 always.
    --
    -- NOTES: Refuses QT the ability to write.
    --------------------------------------------------------------------------------------------------------------------*/
    qint64 writeData(const char *data, qint64 len);

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Read Data
    --
    -- DATE:		April 9th, 2016		REVISIONS:
    --
    -- DESIGNER:	Tyler Trepanier		PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	qint64 readData(char * data, qint64 maxlen);
    --
    -- PARAMETERS:			char *data
    --							Destination of copied data.
    --						qint64 len
    --							Requested amount of data to be copied.
    --
    -- RETURNS:     The amount of data that has been actually copied.
    --
    -- NOTES: Attempts to copy the amount of data request. After the data has been copied, the old voice data will be
    --        removed from the buffer and discarded.
    --------------------------------------------------------------------------------------------------------------------*/
    qint64 readData(char * data, qint64 maxlen);

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Add More Data To Buffer From QByteArray
    --
    -- DATE:		April 9th, 2016		REVISIONS:
    --
    -- DESIGNER:	Tyler Trepanier		PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	void AddMoreDataToBufferFromQByteArray(QByteArray array, qint64 len);
    --
    -- PARAMETERS:			QByteArray array
    --							Source of data to be added onto the buffer.
    --						qint64 len
    --							Length of data to be added.
    --
    -- RETURNS:     The amount of data that has been actually copied.
    --
    -- NOTES: Bypasses the write data to add more data to the buffer. This must occur because QT's methods of writing
    --        data does not work properly when done asynchronously.
    --------------------------------------------------------------------------------------------------------------------*/
    void AddMoreDataToBufferFromQByteArray(QByteArray array, qint64 len);

private:
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Remove Buffered Data
    --
    -- DATE:		April 9th, 2016		REVISIONS:
    --
    -- DESIGNER:	Tyler Trepanier		PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	void RemoveBufferedData(int len);
    --
    -- PARAMETERS:			int len
    --                          Length of data to be removed.
    --
    -- RETURNS:     void
    --
    -- NOTES: Removes the specified length from the buffer and adjusts the read position and the rg_max as well.
    --------------------------------------------------------------------------------------------------------------------*/
    void RemoveBufferedData(int len);

private:
    qint64      rg_readpos; // Reading position of readData
    qint64      rg_max;     // Running total amount data.
    QByteArray  rg_buffer;  // Voice data container.
    bool        playing;    // Flag to indicate if the generator is being used or not.
};

#endif // RECORDGENERATOR_H
