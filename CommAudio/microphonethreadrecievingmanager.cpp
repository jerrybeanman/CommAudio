#include "microphonethreadrecievingmanager.h"
#include <QDebug>

CircularBuffer cbMic;
void MicrophoneThreadRecvManager::MicrohponeRecvThread()
{
    QByteArray temp;
    CBInitialize(&cbMic, 20, 40000);

    if(!clientUDP.InitializeSocket(MIC_PORT))
        return;
    std::cout << "Whomp" << std::endl;
    while(clientUDP.Recv())
    {
        std::cerr << "Mic data received" << std::endl;
        CBPushBack(&cbMic, clientUDP.SocketInfo.Buffer);
        emit RecievedData(clientUDP.SocketInfo.BytesRECV);
    }
    qDebug() << "Exiting mic recv thread";
    CBFree(&cbMic);
}

void MicrophoneThreadRecvManager::closeSocket()
{
    clientUDP.Close();
    this->thread()->exit();
}

void MicrophoneThreadRecvManager::MicrophoneRecvThreadRequest() {
    emit MicrophoneRecvThreadRequested();
}
