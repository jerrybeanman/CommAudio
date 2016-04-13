#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QByteArray>
#include "circularbuffer.h"
#include "Client/ClientUDP.h"
#include "Client/ClientTCP.h"

class UDPThreadManager : public QObject
{
    Q_OBJECT
public:
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: UDPThreadRequest
    --
    -- DATE: March 18, 2016
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE: void UDPThreadRequest()
    --
    -- RETURNS: void.
    --
    -- NOTES:
    -- Emits the signal to start the receive thread.
    ----------------------------------------------------------------------------------------------------------------------*/
    void UDPThreadRequest();
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: closeSocket
    --
    -- DATE: April 4, 2016
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE: void closeSocket()
    --
    -- RETURNS: void.
    --
    -- NOTES:
    -- Closes the socket and thread
    ----------------------------------------------------------------------------------------------------------------------*/
    void closeSocket();
signals:
    void finished();
    void songDataReceived(const unsigned int);
    void UDPThreadRequested();

public slots:
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: receiveThread
    --
    -- DATE: March 18, 2016
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE: void receiveThread()
    --
    -- RETURNS: void.
    --
    -- NOTES:
    -- The UDP thread to receive information from the broadcast channel
    ----------------------------------------------------------------------------------------------------------------------*/
    void UDPReceiveThread();

private:
    ClientUDP clientUDP;
};

#endif // THREADMANAGER_H
