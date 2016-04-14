#ifndef MICROHPONETHREADMANAGER_H
#define MICROHPONETHREADMANAGER_H

#include <QObject>
#include <QByteArray>
#include "circularbuffer.h"
#include "Client/ClientUDP.h"



class MicrophoneThreadManager : public QObject
{
    Q_OBJECT
public:
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	MicrohponeSendThread
    --
    -- DATE:		April 11th, 2016
    --
    -- REVISIONS:
    --
    -- DESIGNER:	Ruoqi Jia
    --
    -- PROGRAMMER:	Ruoqi Jia
    --
    -- INTERFACE:	void MicrophoneThreadManager::MicrohponeSendThread()
    --
    -- RETURNS: void
    --
    -- NOTES: main logic for microhpone thread to send audio data
    --------------------------------------------------------------------------------------------------------------------*/
    MicrophoneThreadManager(QByteArray ip) : ipAddr(ip){}
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	MicrophoneThreadRequest
    --
    -- DATE:		April 11th, 2016
    --
    -- REVISIONS:
    --
    -- DESIGNER:	Ruoqi Jia
    --
    -- PROGRAMMER:	Ruoqi Jia
    --
    -- INTERFACE:	void MicrophoneThreadManager::closeSocket()
    --
    -- RETURNS: void
    --
    -- NOTES: slot that emites a signal
    --------------------------------------------------------------------------------------------------------------------*/
    void MicrophoneThreadRequest();
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	closeSocket
    --
    -- DATE:		April 11th, 2016
    --
    -- REVISIONS:
    --
    -- DESIGNER:	Ruoqi Jia
    --
    -- PROGRAMMER:	Ruoqi Jia
    --
    -- INTERFACE:	void MicrophoneThreadManager::closeSocket()
    --
    -- RETURNS: void
    --
    -- NOTES: close socket for microphone
    --------------------------------------------------------------------------------------------------------------------*/
    void closeSocket();
signals:
    void finished();
    void MicrophoneThreadRequested();

public slots:
    void MicrohponeSendThread();

private:
    bool running = true;
    QByteArray ipAddr;
    ClientUDP clientUDP;
};

#endif
