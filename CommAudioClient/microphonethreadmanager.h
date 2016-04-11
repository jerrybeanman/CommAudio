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
    void MicrophoneThreadRequest();
    void closeSocket();
signals:
    void finished();
    void MicrophoneThreadRequested();

public slots:
    void MicrohponeSendThread();

private:
    ClientUDP clientUDP;
};

#endif
