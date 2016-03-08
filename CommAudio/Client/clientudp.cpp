#include "ClientUDP.h"

bool ClientUDP::InitializeSocket(const char* name, short port)
{
	// Create a WSA v2.2 session
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed with error %d\n", WSAGetLastError());
        return -1;
    }

    // Create socket for writing based on currently selected protocol
    if ((*SockInfo->Socket = WSASocket(AF_INET,SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
        printf("WSASocket() failed with error %d\n", WSAGetLastError());
        return -1;
	}

    InternetAddr.sin_family			= AF_INET;
    InternetAddr.sin_addr.s_addr	= inet_addr(name);
    InternetAddr.sin_port			= htons(port);


}
