#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include "Server/ServerTCP.h"
#include "globals.h"

bool StartFileManager();

DWORD WINAPI TCPAccept(LPVOID lpParameter);
DWORD WINAPI TCPReadThread(LPVOID lpParameter);
#endif // FILEMANAGER_H
