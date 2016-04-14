/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: TCPThreadManager.h - the thread for the tcp connection
--
-- PROGRAM: commAudio
--
-- FUNCTIONS:
--  TCPThreadManager(QByteArray& ip) : ipAddr(ip){}
--  void TCPThreadRequest();
--  void sendSongRequest(QByteArray songName);
--  void TCPReceiveThread();
-- DATE: April 13, 2016
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Scott Plummer
--
-- PROGRAMMER: Scott Plummer
--
-- NOTES:
-- The TCP backend of the program. The tcp handles the file transfer and control data sent between the server and the client.
----------------------------------------------------------------------------------------------------------------------*/
#ifndef TCPTHREADMANAGER_H
#define TCPTHREADMANAGER_H

#include <QObject>
#include "circularbuffer.h"
#include "Client/ClientTCP.h"

class TCPThreadManager : public QObject
{
    Q_OBJECT

public:
    /*------------------------------------------------------------------------------------------------------------------
    -- CTOR: TCPThreadManager
    --
    -- DATE: April 4, 2016
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE:   TCPThreadManager(QByteArray& ip)
    --                      ip: The ipAddress of the server.
    --
    -- NOTES:
    --
    ----------------------------------------------------------------------------------------------------------------------*/
    TCPThreadManager(QByteArray& ip) : ipAddr(ip){}
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: TCPThreadRequest
    --
    -- DATE: April 4, 2016
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE: void TCPThreadRequest()
    --
    -- RETURNS: void.
    --
    -- NOTES:
    -- Emits the signal to start the receive thread.
    ----------------------------------------------------------------------------------------------------------------------*/
    void TCPThreadRequest();
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: sendSongRequest
    --
    -- DATE: April 4, 2016
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE: void sendSongRequest(QByteArray songName)
    --                          songName: name of the song requested
    --
    -- RETURNS: void.
    --
    -- NOTES:
    ----------------------------------------------------------------------------------------------------------------------*/
    void sendSongRequest(QByteArray songName);
signals:
    void TCPThreadRequested();
    void songList(const QByteArray& songList);
    void songNameReceived(const QByteArray name);
    void songHeader();
public slots:
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: TCPReceiveThread
    --
    -- DATE: April 4, 2016
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE: void TCPReceiveThread()
    --
    -- RETURNS: void.
    --
    -- NOTES:
    -- The TCP thread to receive information from the server
    ----------------------------------------------------------------------------------------------------------------------*/
    void TCPReceiveThread();

private:
    QByteArray songName;
    QByteArray ipAddr;
    ClientTCP clientTCP;
};

#endif // TCPTHREADMANAGER_H
