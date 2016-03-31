#ifndef GLOBALS_H
#define GLOBALS_H

#define DEAULT_MULTICAST_IP  "234.5.6.7"
#define DEFAULT_IP           "192.168.0.1"
#define MAX_CLIENTS          10
#define DATA_BUFSIZE         60000
#define DEFAULT_PORT         7000

#include <iostream>
#include <WinSock2.h>
#include <windows.h>
#include <qtcore>
#include "circularbuffer.h"

/* Socket struct for Windows */
typedef struct _SOCKET_INFORMATION
{
    WSAOVERLAPPED  Overlapped;
    SOCKET         Socket;
    CHAR           Buffer[DATA_BUFSIZE];
    WSABUF         DataBuf;
    DWORD          BytesSEND;
    DWORD          BytesRECV;
} SOCKET_INFORMATION, *LPSOCKET_INFORMATION;
extern CircularBuffer cb;
#endif
