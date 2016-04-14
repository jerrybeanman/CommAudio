#include "MicrophoneThreadManager.h"
#include <QDebug>

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	MicrohponeSendThread
--
-- DATE:		April 11th, 2016
--
-- REVISIONS:
--
-- DESIGNER:	Ruoqi Jia
--
-- PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	void MicrophoneThreadManager::MicrohponeSendThread()
--
-- RETURNS: void
--
-- NOTES: main logic for microhpone thread to send audio data
--------------------------------------------------------------------------------------------------------------------*/
void MicrophoneThreadManager::MicrohponeSendThread()
{
    QByteArray temp;

    if(!clientUDP.InitializeSendingSocket(ipAddr.data(), MIC_PORT))
        return;

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
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	closeSocket
--
-- DATE:		April 11th, 2016
--
-- REVISIONS:
--
-- DESIGNER:	Ruoqi Jia
--
-- PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	void MicrophoneThreadManager::closeSocket()
--
-- RETURNS: void
--
-- NOTES: close socket for microphone
--------------------------------------------------------------------------------------------------------------------*/
void MicrophoneThreadManager::closeSocket()
{
    running = false;
    clientUDP.Close();
    this->thread()->exit();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	MicrophoneThreadRequest
--
-- DATE:		April 11th, 2016
--
-- REVISIONS:
--
-- DESIGNER:	Ruoqi Jia
--
-- PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	void MicrophoneThreadManager::closeSocket()
--
-- RETURNS: void
--
-- NOTES: slot that emites a signal
--------------------------------------------------------------------------------------------------------------------*/
void MicrophoneThreadManager::MicrophoneThreadRequest() {
    emit MicrophoneThreadRequested();
}
