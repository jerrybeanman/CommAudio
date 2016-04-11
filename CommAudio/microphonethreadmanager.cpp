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
        if(cb_voice_data.Count > 0)
        {
            char * voicedata = (char *)malloc(DATA_BUFSIZE);
            CBPop(&cb_voice_data, voicedata);
            clientUDP.Send(voicedata, DATA_BUFSIZE);
            free(voicedata);
        }
        // TODO::Implement read from microhpone and assign it into voicedata
    }
}

void MicrophoneThreadManager::closeSocket() {
    clientUDP.Close();
    this->thread()->exit();
}

void MicrophoneThreadManager::MicrophoneThreadRequest() {
    emit MicrophoneThreadRequested();
}
