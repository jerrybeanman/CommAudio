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
    DWORD bytes_sent = 0;
    while(1)
    {
        if(song_size != 0 && *song_size > 0)
        {
            qDebug() << "Incoming data: " << *song_size;
            bytes_sent = *song_size;
            if(!serverUDP->Broadcast(*song_stream_data, &bytes_sent))
            {
                qDebug() << "Jerry, your Broadcast sucks";
                return -1;
            }
            bytes_sent = *song_size ? *song_size = 0 : 0;
            qDebug() << "Data remaining: " << *song_size;
        }
        /*bytes_to_send = udp->bytes_to_send - BytesLeft;

        BytesLeft = udp->bytes_to_send - bytes_to_send;*/
    }
    return 0;
}
