#include "threadmanager.h"
#include <QDebug>
void threadManager::receiveThread() {
    ClientUDP clientUDP;
    if(!clientUDP.InitializeSocket(DEFAULT_PORT))
        return;

    if(!clientUDP.MulticastSettings(DEAULT_MULTICAST_IP))
        return;

    while(1)
    {
        DWORD BytesSent;
        if(!clientUDP.Recv()) {
            return;
        }
        //Song name / size
        //file header data
        //if data is header data emit header signal

        emit dataReceived(clientUDP.SocketInfo.DataBuf.buf, clientUDP.SocketInfo.DataBuf.len);
        qDebug() << "Recieved message: " << clientUDP.SocketInfo.DataBuf.buf;
    }

}

void threadManager::threadRequest() {
    emit threadRequested();
}
