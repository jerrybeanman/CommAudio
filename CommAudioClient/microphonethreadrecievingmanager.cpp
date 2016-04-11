#include "microphonethreadrecievingmanager.h"
#include <QDebug>

CircularBuffer cbMic;
void MicrophoneThreadRecvManager::MicrohponeRecvThread()
{
    QByteArray temp;

    // TODO:: Grab IP from GUI
    /*if(!clientUDP.InitializeSendingSocket( IP  ,DEFAULT_PORT))
        return;*/
    while(1)
    {
        char * voicedata = (char *)malloc(2000);

        // TODO::Implement read from microhpone and assign it into voicedata

        clientUDP.Send(voicedata, strlen(voicedata) + 1);
    }
}

void MicrophoneThreadRecvManager::closeSocket() {
    clientUDP.Close();
    this->thread()->exit();
}

void MicrophoneThreadRecvManager::MicrophoneRecvThreadRequest() {
    emit MicrophoneRecvThreadRequested();
}
