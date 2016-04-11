#include "MicrophoneThreadManager.h"
#include <QDebug>


void MicrophoneThreadManager::MicrohponeSendThread()
{
    QByteArray temp;

    // TODO:: Grab IP from GUI
    /*if(!clientUDP.InitializeSendingSocket( IP  ,DEFAULT_PORT))
        return;*/
    while(1)
    {
        char * voicedata = (char *)malloc(2000);

        // TODO::Implement read from microhpone and assign it into voicedata

        clientUDP.Send(voicedata, 2000);
        free(voicedata);
    }
}

void MicrophoneThreadManager::closeSocket() {
    clientUDP.Close();
    this->thread()->exit();
}

void MicrophoneThreadManager::MicrophoneThreadRequest() {
    emit MicrophoneThreadRequested();
}
