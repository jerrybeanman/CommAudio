#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include "globals.h"
#include "Server/ServerUDP.h"

bool StartSoundManager();

DWORD WINAPI BroadcastMusic(LPVOID lpParameter);

#endif // SOUNDMANAGER_H
