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
    void sendSongRequest(QByteArray& songName);
    ClientTCP clientTCP;
signals:
    void TCPThreadRequested();
    void songList(const QByteArray& songList);

public slots:
    void TCPReceiveThread();

private:
    QByteArray songName;
    QByteArray ipAddr;
};

#endif // TCPTHREADMANAGER_H
