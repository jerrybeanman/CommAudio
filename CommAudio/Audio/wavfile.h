/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: wavfile.h - Voice Data Container for reading
--
-- PROGRAM:     commAudio
--
-- FUNCTIONS:
--              bool open(const QString &fileName);
--              const QAudioFormat &fileFormat() const;
--              qint64 headerLength() const;
--              bool readHeader();
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
-- Wave file that takes in only wave files and has the capability to read the header to check for a valid file
-- format.
----------------------------------------------------------------------------------------------------------------------*/

#ifndef WAVFILE_H
#define WAVFILE_H

#include <QObject>
#include <QFile>
#include <QAudioFormat>

class WavFile : public QFile
{
public:
    WavFile(QObject *parent = 0);

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Open
    --
    -- DATE:		March 18th, 2016    REVISIONS:
    --
    -- DESIGNER:	Tyler Trepanier		PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	bool open(const QString &fileName);
    --
    -- PARAMETERS:	const QString &fileName
    --					File source which will be attempted to open the file.
    --
    -- RETURNS:     True on success, false otherwise.
    --
    -- NOTES: Opens the file for IO operation.
    --------------------------------------------------------------------------------------------------------------------*/
    using QFile::open;
    bool open(const QString &fileName);

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
    -- NOTES: Returns a reference to the assigned file format.
    --------------------------------------------------------------------------------------------------------------------*/
    const QAudioFormat &fileFormat() const;

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Header Length
    --
    -- DATE:		March 9th, 2016             REVISIONS:
    --
    -- DESIGNER:	Tyler Trepanier				PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	qint64 headerLength() const;
    --
    -- RETURNS: Returns the length of the header.
    --
    -- NOTES: Returns the length of the wave file header.
    --------------------------------------------------------------------------------------------------------------------*/
    qint64 headerLength() const;

private:
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Read Headers
    --
    -- DATE:		March 9th, 2016             REVISIONS:
    --
    -- DESIGNER:	Tyler Trepanier				PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	bool readHeader();
    --
    -- RETURNS: True if the file header is valid, false otherwise.
    --
    -- NOTES: Attempts to read the wave file's header. Returns failure or success and ensures the file won't be played.
    --------------------------------------------------------------------------------------------------------------------*/
    bool readHeader();

private:
    QAudioFormat m_fileFormat;  // Wave file song playback information.
    qint64 m_headerLength;      // Length of the file's header.
};

#endif // WAVFILE_H
