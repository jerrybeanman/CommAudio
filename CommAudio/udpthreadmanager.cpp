#include "udpthreadmanager.h"
#include <QDebug>


void UDPThreadManager::UDPReceiveThread() {
    ClientUDP clientUDP;
    QByteArray temp;
    if(!clientUDP.InitializeSocket(DEFAULT_PORT))
        return;

    if(!clientUDP.MulticastSettings(DEAULT_MULTICAST_IP))
        return;

    CBInitialize(&cb, 10, 40000);
    bool first = true;
    while(1)
    {
        if(!clientUDP.Recv()) {
            return;
        }

        //Song name / size
        //file header data
        //if data is header data emit header signal
        CBPushBack(&cb, clientUDP.SocketInfo.DataBuf.buf);
        if(first) {
            emit songHeader(clientUDP.SocketInfo.BytesRECV);
            first = false;
        } else if(clientUDP.SocketInfo.DataBuf.buf[0] == (char)18) {

        } else {
            emit songDataReceived(clientUDP.SocketInfo.BytesRECV);
        }
    }


}


void UDPThreadManager::UDPThreadRequest() {
    emit UDPThreadRequested();
}
