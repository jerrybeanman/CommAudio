#ifndef MICROHPONETHREADRECVMANAGER_H
#define MICROHPONETHREADRECVMANAGER_H

#include <QObject>
#include <QByteArray>
#include "circularbuffer.h"
#include "Client/ClientUDP.h"

class MicrophoneThreadRecvManager : public QObject
{
    Q_OBJECT
public:
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	MicrohponeRecvThread
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
    -- NOTES: main logic for microhpone thread to recieve audio data
    --------------------------------------------------------------------------------------------------------------------*/
    MicrophoneThreadRecvManager(QByteArray ip) : ipAddr(ip){}

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
    -- INTERFACE:	void MicrophoneThreadRecvManager::closeSocket()
    --
    -- RETURNS: void
    --
    -- NOTES: close socket
    --------------------------------------------------------------------------------------------------------------------*/
    void MicrophoneRecvThreadRequest();
    void closeSocket();
signals:
    void finished();
    void MicrophoneRecvThreadRequested();
    void RecievedData(const unsigned int);

public slots:
    void MicrohponeRecvThread();

private:
    QByteArray ipAddr;
    ClientUDP clientUDP;
};

#endif
