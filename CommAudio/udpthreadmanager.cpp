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

        //Song name / size
        //file header data
        //if data is header data emit header signal
        QByteArray header = QByteArray::fromRawData(clientUDP.SocketInfo.DataBuf.buf,
                                                    7);
        if(header.startsWith("HEADER:")) {
            CBPushBack(&cb, clientUDP.SocketInfo.DataBuf.buf + 7);
            emit songHeader(clientUDP.SocketInfo.BytesRECV);
        } else if(clientUDP.SocketInfo.DataBuf.buf[0] == (char)18) {

        } else {
            CBPushBack(&cb, clientUDP.SocketInfo.DataBuf.buf);
            emit songDataReceived(clientUDP.SocketInfo.BytesRECV);
        }
    }
    qDebug() << "UDP Thread exiting";
    CBFree(&cb);
}

void UDPThreadManager::closeSocket() {
    clientUDP.Close();
    this->thread()->exit();
}

void UDPThreadManager::UDPThreadRequest() {
    emit UDPThreadRequested();
}
