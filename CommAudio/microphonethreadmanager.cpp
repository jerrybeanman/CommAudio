#include "MicrophoneThreadManager.h"
#include <QDebug>


void MicrophoneThreadManager::MicrohponeSendThread()
{
    QByteArray temp;

    // TODO:: Grab IP from GUI
    if(!clientUDP.InitializeSendingSocket(ipAddr.data(),DEFAULT_PORT))
        return;

    std::cerr << "Stomp" << std::endl;
    std::cout << "Running:" << running << " count:" << cb_voice_data.Count << std::endl;
    while(running)
    {
        if(cb_voice_data.Count > 0)
        {

            char * voicedata = (char *)malloc(DATA_BUFSIZE);
            CBPop(&cb_voice_data, voicedata);
            if(!clientUDP.Send(voicedata, DATA_BUFSIZE)) {
               free(voicedata);
               break;
            }
            std::cerr << "Mic data sent" << std::endl;
            free(voicedata);
        }
        // TODO::Implement read from microhpone and assign it into voicedata
    }

    qDebug() << "exiting mic send thread";
}

void MicrophoneThreadManager::closeSocket() {
    running = false;
    clientUDP.Close();
    this->thread()->exit();
}

void MicrophoneThreadManager::MicrophoneThreadRequest() {
    emit MicrophoneThreadRequested();
}
