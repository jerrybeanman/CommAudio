#ifndef COMPLETIONROUTINE_H
#define COMPLETIONROUTINE_H
#include "ClientTCP.h"
void CALLBACK WorkerRoutine(DWORD Error, DWORD BytesTransferred,
    LPWSAOVERLAPPED Overlapped, DWORD InFlags);
void initializeConnection(ClientTCP& client);
#endif // COMPLETIONROUTINE_H
