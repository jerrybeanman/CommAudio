//420 scamaz
#include "threadmanager.h"
#include <QDebug>
void threadManager::receiveThread() {
    ClientUDP clientUDP;
    QByteArray temp;
    int packet_size = 16384;
    if(!clientUDP.InitializeSocket(DEFAULT_PORT))
        return;

    if(!clientUDP.MulticastSettings(DEAULT_MULTICAST_IP))
        return;

    while(1)     //always scamazing
    {
        if(!clientUDP.Recv()) {
            return;
        }

        //Song name / size
        //file header data
        //if data is header data emit header signal

        emit dataReceived(clientUDP.SocketInfo.DataBuf.buf, clientUDP.SocketInfo.BytesRECV);
    }

}

void threadManager::threadRequest() {
    emit threadRequested();
}
