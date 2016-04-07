#ifndef TCPTHREADMANAGER_H
#define TCPTHREADMANAGER_H

#include <QObject>
#include "circularbuffer.h"
#include "Client/ClientTCP.h"

class TCPThreadManager : public QObject
{
    Q_OBJECT

public:
    TCPThreadManager(QByteArray& ip) : ipAddr(ip){}
    void TCPThreadRequest();

signals:
    void TCPThreadRequested();

public slots:
    void TCPReceiveThread();

private:
    QByteArray ipAddr;
};

#endif // TCPTHREADMANAGER_H
