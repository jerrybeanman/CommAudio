#include "microphonethreadrecievingmanager.h"
#include <QDebug>

CircularBuffer cbMic;
void MicrophoneThreadRecvManager::MicrohponeRecvThread()
{
    QByteArray temp;
    CBInitialize(&cbMic, 20, 40000);

    // TODO:: Grab IP from GUI
    /*if(!clientUDP.InitializeSendingSocket( IP  ,DEFAULT_PORT))
        return;*/
    while(clientUDP.Recv())
    {
      CBPushBack(&cbMic, clientUDP.SocketInfo.Buffer);
      emit RecievedData(clientUDP.SocketInfo.BytesRECV);
    }
    qDebug() << "Exiting mic recv thread";
    CBFree(&cbMic);
}

void MicrophoneThreadRecvManager::closeSocket() {
    clientUDP.Close();
    this->thread()->exit();
}

void MicrophoneThreadRecvManager::MicrophoneRecvThreadRequest() {
    emit MicrophoneRecvThreadRequested();
}
