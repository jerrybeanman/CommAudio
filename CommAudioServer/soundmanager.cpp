#include "soundmanager.h"
ServerUDP serverUDP;
bool StartSoundManager(struct UDPBroadcast* udp)
{
    DWORD UDPServerThreadID;

    if(serverUDP.InitializeSocket(DEFAULT_PORT) < 0)
        return FALSE;

    if(serverUDP.MulticastSettings(DEAULT_MULTICAST_IP) < 0)
        return FALSE;

    udp->serverUDP = serverUDP;

    if(CreateThread(NULL, 0, BroadcastMusic, (LPVOID)&udp, 0, &UDPServerThreadID) == NULL)
    {
        std::cout << "StartSoundManager()::Thread creation for BroadcastMusic() failed " << std::endl;
    	return FALSE;
    }
    return TRUE;
}

DWORD WINAPI BroadcastMusic(LPVOID lpParameter)
{
    std::cout << "Thread created " << std::endl;

    struct UDPBroadcast* udp = (struct UDPBroadcast*)lpParameter;

    DWORD bytes_to_send = udp->bytes_to_send;
    DWORD BytesLeft = 0;
    char* source = udp->source;
    ServerUDP serverUDP = udp->serverUDP;
    while(1)
    {
        bytes_to_send = udp->bytes_to_send - BytesLeft;
        if(!serverUDP.Broadcast(source, &bytes_to_send))
            return -1;
        BytesLeft = udp->bytes_to_send - bytes_to_send;
    }
    free(udp);
    return 0;
}
