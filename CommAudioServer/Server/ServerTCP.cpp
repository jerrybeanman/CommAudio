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
    LocalAddr.sin_port = htons(DEFAULT_PORT_TCP);

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

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	Accept
--
-- DATE:		Febuary 28th, 2016		REVISIONS:
--
-- DESIGNER:	Ruoqi Jia				PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	std::string ServerTCP::Accept(void)
--
-- RETURNS: void
--
-- NOTES: Accepts a client, store its ip and socket and to ClientList
--------------------------------------------------------------------------------------------------------------------*/
std::string ServerTCP::Accept(void)
{
    Client newClient;
    int Len = sizeof(newClient.Connection);
    if((newClient.SocketInfo.Socket = accept(SocketInfo.Socket, NULL, NULL)) == INVALID_SOCKET)
    {
        std::cerr << "ServerTCP::Accept() failed with error " << WSAGetLastError() << std::endl;
        return "";
    }
    std::string ip = inet_ntoa(newClient.Connection.sin_addr);
    ClientList[ip] = newClient;
    return ip;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	Broadcast
--
-- DATE:		Febuary 28th, 2016		REVISIONS:
--
-- DESIGNER:	Ruoqi Jia				PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	bool ServerTCP::Broadcast(char *message, LPDWORD lpNumberOfBytesSent)
--
-- RETURNS: void
--
-- NOTES: Broadcast a message to all connected clients in ClientList
--------------------------------------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	Send
--
-- DATE:		Febuary 28th, 2016		REVISIONS:
--
-- DESIGNER:	Ruoqi Jia				PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	void ServerTCP::Send(LPSOCKET_INFORMATION SockInfo, char * message)
--                  SocketInfo  :   Specific socket to send message to
--                  message     :   Data to send
--
-- RETURNS: void
--
-- NOTES: Send data to a specific client in ClientList
--------------------------------------------------------------------------------------------------------------------*/
void ServerTCP::Send(LPSOCKET_INFORMATION SockInfo, char * message)
{
    ZeroMemory(&SockInfo->Overlapped, sizeof(WSAOVERLAPPED));
    SockInfo->Overlapped.hEvent = WSACreateEvent();

    /* Post a message back to the socket for aknowledgement */
    if (WSASend(SockInfo->Socket,    /* Writing socket                       */
        &(SockInfo->DataBuf),        /* Pointer to WSABUF                    */
        1,                           /* Only 1 WSABUF                        */
        &SockInfo->BytesSEND,        /* Bytes that are sent                  */
        0,                           /* No behavior to modify                */
        &(SockInfo->Overlapped),     /* Pointer to overlapped struct         */
        NULL)                        /* No completion routine                */
        == SOCKET_ERROR)
    {
        if (WSAGetLastError() != ERROR_IO_PENDING)
        {
            std::cerr << "Send() failed with errno: " << WSAGetLastError() << std::endl;
            return;
        }
        if(WSAWaitForMultipleEvents(1, &SockInfo->Overlapped.hEvent, FALSE, INFINITE, FALSE) == WAIT_TIMEOUT)
        {
            std::cerr << "ServerUDP::WSASendto() Tmeout " << std::endl;
            return;
        }
    }
    if(!WSAGetOverlappedResult(SockInfo->Socket, &(SockInfo->Overlapped), &SockInfo->BytesSEND, FALSE, &flags))
    {
        std::cerr << "ServerUDP::WSAGetOverlappedResult() failed: " << WSAGetLastError() << std::endl;
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	RoutineManager
--
-- DATE:		Febuary 28th, 2016		REVISIONS:
--
-- DESIGNER:	Ruoqi Jia				PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	void RoutineManager(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags);
--                      ~Error				: Error code
--                      ~BytesTransffered	: Total bytes recieved from packet
--                      ~Overlapped			: Overlapped structure
--                      ~InFlags            : Modification flags
-- RETURNS: void
--
-- NOTES: Callback completion routine for recv when a packet has been recieved.
--------------------------------------------------------------------------------------------------------------------*/
void ServerTCP::RoutineManager(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags)
{

}

