#pragma once
#include <iostream>
#include <sstream>
#include <math.h>
#include <limits.h>
#include <WinSock2.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <commctrl.h>
#include <time.h>

#define DATA_BUFSIZE 2000

/* Socket struct for Windows */
typedef struct _SOCKET_INFORMATION
{
    WSAOVERLAPPED * Overlapped;
    SOCKET        * Socket;
    CHAR            Buffer[DATA_BUFSIZE];
    WSABUF        * DataBuf;
    DWORD           BytesSEND;
    DWORD           BytesRECV;
} SOCKET_INFORMATION, *LPSOCKET_INFORMATION;
