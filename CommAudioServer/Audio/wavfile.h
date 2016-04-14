/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

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
