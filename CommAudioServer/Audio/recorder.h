#ifndef RECORDER_H
#define RECORDER_H

#include <QApplication>
#include <QAudioInput>
#include <QAudioDeviceInfo>
#include <QDebug>
#include "globals.h"

class Recorder : public QIODevice
{
    Q_OBJECT
public:
    Recorder();
    ~Recorder();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	File Format
    --
    -- DATE:		March 9th, 2016             REVISIONS:
    --
    -- DESIGNER:	Tyler Trepanier				PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	const QAudioFormat &fileFormat() const;
    --
    -- RETURNS:     A reference to the recording format.
    --
    -- NOTES: Returns a reference to the QAudioFormat which is the format being recorded.
    --------------------------------------------------------------------------------------------------------------------*/
    const QAudioFormat &fileFormat() const;

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Start
    --
    -- DATE:		March 18th, 2016            REVISIONS:  April 9th, 2016 (Tyler Trepanier)
    --                                                          -Implemented network functionality
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
    -- DATE:		March 18th, 2016            REVISIONS:
    --
    -- DESIGNER:	Tyler Trepanier				PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	void stop();
    --
    -- RETURNS: void
    --
    -- NOTES: Stops the Recorder from reading from the input device and deletes the input device.
    --------------------------------------------------------------------------------------------------------------------*/
    void stop();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Write Data
    --
    -- DATE:		April 9th, 2016		REVISIONS:  April 11th, 2016 (Tyler Trepanier)
    --                                                  Input device writes to the circular buffer instead.
    --
    -- DESIGNER:	Tyler Trepanier		PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	qint64 writeData(const char *data, qint64 len);
    --
    -- PARAMETERS:			const char *data
    --							Data source which contains voice data.
    --						qint64 len
    --							Length of the data source
    --
    -- RETURNS:     Length of the message writen (always writes the len requested)
    --
    -- NOTES: Uses the QAudioInput device and writes to the circular buffer. Every successful write sends a signal to
    --        be caught by the class using this.
    --------------------------------------------------------------------------------------------------------------------*/
    qint64 writeData(const char *data, qint64 len);

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Read Data
    --
    -- DATE:		March 18th, 2016	REVISIONS:  April 11th, 2016 (Tyler Trepanier)
    --                                                  Removed read functionality, reimplemented with a
    --                                                  circaular buffer.
    --
    -- DESIGNER:	Tyler Trepanier		PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	qint64 readData(char * data, qint64 maxlen);
    --
    -- PARAMETERS:			char *data
    --							Unused element.
    --						qint64 len
    --							Unused element
    --
    -- RETURNS:     Always zero, reading is not allowed.
    --
    -- NOTES: Denies QT the ability to read data by overriding this method.
    --------------------------------------------------------------------------------------------------------------------*/
    qint64 readData(char * data, qint64 maxlen);

private:
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Set Format
    --
    -- DATE:		March 18th, 2016            REVISIONS:
    --
    -- DESIGNER:	Tyler Trepanier				PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	bool SetFormat();
    --
    -- RETURNS: Returns whether or not setting the format was a success.
    --
    -- NOTES: Returns true or false depending if the format was set successfully.
    --------------------------------------------------------------------------------------------------------------------*/
    bool SetFormat();

private:
    QAudioFormat        r_format;           // Recorder's audio format
    QAudioDeviceInfo    r_inputInfo;        // Verifies the r_input's format is acceptable
    QAudioInput         *r_input;           // Input device to record voice data
    QByteArray          r_buffer;           // Voice data container
    bool                inProgress;         // Flag to indicate if the device is operating or not.
    int                 r_bytes_AVAILABLE;  // Integer used to signal the quantity of data available after writing.
signals:

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	dataAvailable
    --
    -- DATE:		April 11th, 2016            REVISIONS:
    --
    -- DESIGNER:	Tyler Trepanier				PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	void dataAvailable(int len);
    --
    -- RETURNS:     void
    --
    -- NOTES: Signal sent to indicate that there was voice data collected and ready for processing.
    --------------------------------------------------------------------------------------------------------------------*/
    void dataAvailable(int len);

};

#endif // RECORDER_H
