#include "soundmanager.h"
ServerUDP serverUDP;
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

DWORD WINAPI BroadcastMusic(LPVOID lpParameter)
{
    std::cout << "Thread created " << std::endl;
    ServerUDP * serverUDP = (ServerUDP * )lpParameter;
    while(1)
    {
        DWORD BytesSent;
        if(!serverUDP->Broadcast("Scamaz", &BytesSent))
            return -1;
    }
    return 0;
}
