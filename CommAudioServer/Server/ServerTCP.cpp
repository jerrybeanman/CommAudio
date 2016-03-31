#include "ServerTCP.h"
#include <iostream>
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	InitializeSocket
--
-- DATE:		Febuary 28th, 2016		REVISIONS:
--
-- DESIGNER:	Ruoqi Jia				PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	virtual int InitializeSocket(short port) = 0;
--						~port: port number of peer host
--
-- RETURNS: void
--
-- NOTES: Initialize socket, server address to lookup to, and connect to the server
--------------------------------------------------------------------------------------------------------------------*/
bool ServerTCP::InitializeSocket(short port)
{
    // Create a WSA v2.2 session
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed with error" << WSAGetLastError() << std::endl;
        return false;
    }

    // Create socket for listening
    if ((SocketInfo.Socket = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        std::cerr << "WSASocket() failed with error " << WSAGetLastError() << std::endl;
        return false;
    }

    // Initialize address structure
    LocalAddr.sin_family = AF_INET;
    LocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    LocalAddr.sin_port = htons(DEFAULT_PORT);

    // Bind address to the listening socket
    if (bind(SocketInfo.Socket, (PSOCKADDR)&LocalAddr, sizeof(LocalAddr)) == SOCKET_ERROR)
    {
        std::cerr << "bind() failed with error " << WSAGetLastError() << std::endl;
        return false;
    }

    // listens for only 1 connection
    if (listen(SocketInfo.Socket, MAX_CLIENTS) == SOCKET_ERROR)
    {
        std::cerr << "listen() failed with error " <<  WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}

std::string ServerTCP::Accept(void)
{
    Client newClient;
    int Len = sizeof(newClient.Connection);
    if((newClient.SocketInfo.Socket = accept(ListeningSocket, (SOCKADDR*)&newClient.Connection, &Len)) == INVALID_SOCKET)
    {
        std::cerr << "ServerTCP::Accept() failed with error " << WSAGetLastError() << std::endl;
        return "";
    }
    std::string ip = inet_ntoa(newClient.Connection.sin_addr);
    ClientList[ip] = newClient;
    return ip;
}



bool ServerTCP::Broadcast(char *message, LPDWORD lpNumberOfBytesSent)
{
    Client tmpClient;
    for(const auto &pair : ClientList)
    {
        tmpClient = pair.second;
        if(send(tmpClient.SocketInfo.Socket, message, DATA_BUFSIZE, 0) == -1)
        {
            std::cerr << "Broadcast() failed for player id: " << pair.first << std::endl;
            std::cerr << "errno: " << WSAGetLastError() << std::endl;
            return false;
        }
    }
    return true;
}

void ServerTCP::Send(LPSOCKET_INFORMATION SockInfo, char * message)
{

}

void ServerTCP::RoutineManager(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags)
{

}

