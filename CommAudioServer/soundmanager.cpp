#include "soundmanager.h"
ServerUDP serverUDP;
bool StartSoundManager()
{
    DWORD UDPServerThreadID;

    if(serverUDP.InitializeSocket(DEFAULT_PORT) < 0)
        return FALSE;

    if(serverUDP.MulticastSettings(DEAULT_MULTICAST_IP) < 0)
        return FALSE;

    //udp->serverUDP = serverUDP;

    if(CreateThread(NULL, 0, BroadcastMusic, (LPVOID)&serverUDP, 0, &UDPServerThreadID) == NULL)
    {
        std::cout << "StartSoundManager()::Thread creation for BroadcastMusic() failed " << std::endl;
    	return FALSE;
    }
    return TRUE;
}

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

            //std::cerr << "Sending::" << std::endl;
            /*for(DWORD i = 0; i < bytes_to_SEND; i++)
            {
                fprintf(stderr, "%c", (*song_stream_data)[i]);
            }
            fprintf(stderr, "\n");
            fflush(stderr);*/

            //Make extra sure that we don't send garbage data
            if(song_stream_data != 0 && !serverUDP->Broadcast(*song_stream_data, &bytes_to_SEND))
            {
                qDebug() << "Jerry, your Broadcast sucks";
                std::cout << "SoundManager::Broadcast>>Song size after:" << *song_size << " bytes_to_SEND:" << bytes_to_SEND << std::endl;
                return -1;
            }
            //std::cout << "SoundManager::Broadcast>>Song size before:" << *song_size << " bytes_to_SEND:" << bytes_to_SEND << std::endl;
            *song_size -= bytes_to_SEND;
            *song_stream_data += bytes_to_SEND;
            std::cout << "SoundManager::Broadcast>>Song size after:" << *song_size << " bytes_to_SEND:" << bytes_to_SEND << std::endl;
        }

    }
    return 0;
}
