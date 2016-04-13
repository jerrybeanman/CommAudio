#include "soundmanager.h"
ServerUDP serverUDP;
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	StartSoundManagers
--
-- DATE:		April 11th, 2016
--
-- REVISIONS:
--
-- DESIGNER:	Ruoqi Jia
--
-- PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	bool StartSoundManager()
--
-- RETURNS: True on succesfull execution on UDP socket, false otherwise
--
-- NOTES: Initialize socket and creates the accepting thread
--------------------------------------------------------------------------------------------------------------------*/
bool StartSoundManager()
{
    DWORD UDPServerThreadID;

    if(serverUDP.InitializeSocket(DEFAULT_PORT) < 0)
        return FALSE;

    if(serverUDP.MulticastSettings(DEAULT_MULTICAST_IP) < 0)
        return FALSE;

    if(CreateThread(NULL, 0, BroadcastMusic, (LPVOID)&serverUDP, 0, &UDPServerThreadID) == NULL)
    {
        std::cout << "StartSoundManager()::Thread creation for BroadcastMusic() failed " << std::endl;
    	return FALSE;
    }
    return TRUE;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	BroadcastMusic
--
-- DATE:		April 11th, 2016
--
-- REVISIONS:
--
-- DESIGNER:	Ruoqi Jia
--
-- PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	DWORD WINAPI BroadcastMusic(LPVOID lpParameter)
--
-- RETURNS: Thread exit code
--
-- NOTES: Initialize thread for broacasting music data to the multicast channel
--------------------------------------------------------------------------------------------------------------------*/
DWORD WINAPI BroadcastMusic(LPVOID lpParameter)
{
    std::cout << "Thread created " << std::endl;

    ServerUDP* serverUDP = (ServerUDP*)lpParameter;
    DWORD bytes_to_SEND = 0;
    while(1)
    {
        if(song_size != 0 && *song_size > 0 && song_stream_data != 0)
        {
            bytes_to_SEND = (DWORD)(*song_size);
            if(bytes_to_SEND > DATA_BUFSIZE)
            {
                bytes_to_SEND = DATA_BUFSIZE;
            }

            //Make extra sure that we don't send garbage data
            if(song_stream_data != 0 && !serverUDP->Broadcast(*song_stream_data, bytes_to_SEND))
            {
                return -1;
            }

            *song_size -= bytes_to_SEND;
            *song_stream_data += bytes_to_SEND;
        }

    }
    return 0;
}
