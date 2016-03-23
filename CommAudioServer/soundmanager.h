#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include "globals.h"
#include "datagenerator.h"
#include "Server/ServerUDP.h"

struct UDPBroadcast
{
    ServerUDP serverUDP;
    DWORD bytes_to_send;
    char* source;
};

bool SetDataSource(DataGenerator* generator);
bool StartSoundManager();

DWORD WINAPI BroadcastMusic(LPVOID lpParameter);

#endif // SOUNDMANAGER_H
