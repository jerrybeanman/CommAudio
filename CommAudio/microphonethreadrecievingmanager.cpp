#include "microphonethreadrecievingmanager.h"
#include <QDebug>

CircularBuffer cbMic;

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	MicrohponeRecvThread
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
-- NOTES: main logic for microhpone thread to recieve audio data
--------------------------------------------------------------------------------------------------------------------*/
void MicrophoneThreadRecvManager::MicrohponeRecvThread()
{
    QByteArray temp;
    CBInitialize(&cbMic, 20, 40000);

    if(!clientUDP.InitializeSocket(MIC_PORT))
        return;
    while(clientUDP.Recv())
    {
        std::cerr << "Mic data received" << std::endl;
        CBPushBack(&cbMic, clientUDP.SocketInfo.Buffer);
        emit RecievedData(clientUDP.SocketInfo.BytesRECV);
    }
    CBFree(&cbMic);
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
-- INTERFACE:	void MicrophoneThreadRecvManager::closeSocket()
--
-- RETURNS: void
--
-- NOTES: close socket
--------------------------------------------------------------------------------------------------------------------*/
void MicrophoneThreadRecvManager::closeSocket()
{
    clientUDP.Close();
    this->thread()->exit();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	MicrophoneRecvThreadRequest
--
-- DATE:		April 11th, 2016
--
-- REVISIONS:
--
-- DESIGNER:	Ruoqi Jia
--
-- PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	void MicrophoneThreadRecvManager::MicrophoneRecvThreadRequest()
--
-- RETURNS: void
--
-- NOTES: slot that emits a signal
--------------------------------------------------------------------------------------------------------------------*/
void MicrophoneThreadRecvManager::MicrophoneRecvThreadRequest()
{
    emit MicrophoneRecvThreadRequested();
}
