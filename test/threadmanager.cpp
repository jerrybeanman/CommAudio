//420 scamaz
#include "threadmanager.h"
#include <QDebug>
#include "circularbuffer.h"
void threadManager::receiveThread() {
    ClientUDP clientUDP;
    QByteArray temp;
    int packet_size = 16384;
    if(!clientUDP.InitializeSocket(DEFAULT_PORT))
        return;

    if(!clientUDP.MulticastSettings(DEAULT_MULTICAST_IP))
        return;

    /*QFile* data_file = new QFile("output.wav");
    if(!data_file->open(QIODevice::WriteOnly | QIODevice::Append)) {
        return;
    }*/

    CBInitialize(&cb, 10, 40000);

    while(1)     //always scamazing
    {
        if(!clientUDP.Recv()) {
            return;
        }

        //Song name / size
        //file header data
        //if data is header data emit header signal
       // QByteArray test;
        //test.setRawData(clientUDP.SocketInfo.DataBuf.buf, clientUDP.SocketInfo.BytesRECV);
        CBPushBack(&cb, clientUDP.SocketInfo.DataBuf.buf);
        emit dataReceived(clientUDP.SocketInfo.BytesRECV);
        //QDataStream stream(data_file);
        //stream.writeRawData(clientUDP.SocketInfo.DataBuf.buf, clientUDP.SocketInfo.BytesRECV);
    }


}

void threadManager::threadRequest() {
    emit threadRequested();
}

//scamaz
