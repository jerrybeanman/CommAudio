#include "microphonethreadrecievingmanager.h"
#include <QDebug>

CircularBuffer cbMic;
void MicrophoneThreadRecvManager::MicrohponeRecvThread()
{
    QByteArray temp;
    CBInitialize(&cbMic, 10, 2000);
    // TODO:: Grab IP from GUI
    /*if(!clientUDP.InitializeSendingSocket( IP  ,DEFAULT_PORT))
        return;*/
    while(clientUDP.Recv())
    {
      CBPushBack(&cbMic, clientUDP.SocketInfo.Buffer);
      emit RecievedData(clientUDP.SocketInfo.BytesRECV);
    }
}

void MicrophoneThreadRecvManager::closeSocket() {
    clientUDP.Close();
    this->thread()->exit();
}

void MicrophoneThreadRecvManager::MicrophoneRecvThreadRequest() {
    emit MicrophoneRecvThreadRequested();
}
