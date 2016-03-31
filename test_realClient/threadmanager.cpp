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

    QFile* data_file = new QFile("output.wav");
    if(!data_file->open(QIODevice::WriteOnly | QIODevice::Append)) {
        return;
    }

    while(1)     //always scamazing
    {
        if(!clientUDP.Recv()) {
            return;
        }

        //Song name / size
        //file header data
        //if data is header data emit header signal
        for(DWORD i = 0; i < clientUDP.SocketInfo.BytesRECV; i++)
        {
            fprintf(stderr, "%c", clientUDP.SocketInfo.DataBuf.buf[i]);
        }
        fprintf(stderr, "\n");
        QByteArray test;
        test.setRawData(clientUDP.SocketInfo.DataBuf.buf, clientUDP.SocketInfo.BytesRECV);
        //emit dataReceived(test, clientUDP.SocketInfo.BytesRECV);
        QDataStream stream(data_file);
        stream.writeRawData(clientUDP.SocketInfo.DataBuf.buf, clientUDP.SocketInfo.BytesRECV);
    }

}

void threadManager::threadRequest() {
    emit threadRequested();
}
