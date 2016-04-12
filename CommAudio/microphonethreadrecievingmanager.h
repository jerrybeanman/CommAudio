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
    MicrophoneThreadRecvManager(QByteArray ip) : ipAddr(ip){}
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
