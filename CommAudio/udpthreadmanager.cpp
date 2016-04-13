#include "udpthreadmanager.h"
#include <QDebug>


void UDPThreadManager::UDPReceiveThread() {
    QByteArray temp;
    if(!clientUDP.InitializeSocket(DEFAULT_PORT))
        return;

    if(!clientUDP.MulticastSettings(DEAULT_MULTICAST_IP))
        return;

    CBInitialize(&cb, 10, DATA_BUFSIZE);
    while(clientUDP.Recv())
    {
        std::cerr << "Song data received" << std::endl;

        CBPushBack(&cb, clientUDP.SocketInfo.DataBuf.buf);
        emit songDataReceived(clientUDP.SocketInfo.BytesRECV);
    }
    qDebug() << "UDP Thread exiting";
    CBFree(&cb);
}

void UDPThreadManager::closeSocket() {
    clientUDP.leaveMulticast();
    clientUDP.Close();
    this->thread()->exit();
}

void UDPThreadManager::UDPThreadRequest() {
    emit UDPThreadRequested();
}
