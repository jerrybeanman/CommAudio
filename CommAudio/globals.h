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

<<<<<<< HEAD
=======
#define DEFAULT_IP   "192.168.0.1"
>>>>>>> 0febc20486465f324a487855456c8ed0b4679533
#define MAX_CLIENTS  10
#define DATA_BUFSIZE 2000
#define DEFAULT_PORT 7000

#include "circularbuffer.h"

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
