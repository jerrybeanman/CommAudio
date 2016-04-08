#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QByteArray>
#include "globals.h"
#include "Client/clientudp.h"

class threadManager : public QObject
{
    Q_OBJECT
public:
    explicit threadManager(QObject *parent = 0) {}

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: threadRequest
    --
    -- DATE: March 18, 2015
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE: void threadRequest()
    --
    -- RETURNS: void.
    --
    -- NOTES:
    -- Emits the signal to start the receive thread.
    ----------------------------------------------------------------------------------------------------------------------*/
    void threadRequest();
signals:
    void finished();
    void dataReceived(QByteArray array, qint64 len);
    void dataHeader(const QByteArray& header);
    void threadRequested();
    void playlistReceived(const QByteArray& playlist);

public slots:
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: receiveThread
    --
    -- DATE: March 18, 2015
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
    void receiveThread();
};

#endif // THREADMANAGER_H
