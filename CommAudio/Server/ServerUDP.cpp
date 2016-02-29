#include "ServerUDP.h"
int ServerUDP::InitializeSocket(short port)
{
    // Create a WSA v2.2 session
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed with error %d\n", WSAGetLastError());
        return -1;
    }

    // Create socket for listening
    if ((ServerSocket = socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP) == INVALID_SOCKET))
    {
        printf("WSASocket() failed with error %d\n", WSAGetLastError());
        return -1;
    }

    // Initialize address structure
    InternetAddr.sin_family = AF_INET;
    InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    InternetAddr.sin_port = htons(port);

    // Bind address to the listening socket
    if (bind(ServerSocket, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)
    {
        printf("bind() failed with error %d\n", WSAGetLastError());
        return -1;
    }
    return 0;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	Broadcast
--
-- DATE:		Febuary 28th, 2016		REVISIONS:
--
-- DESIGNER:	Ruoqi Jia				PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	virtual void Broadcast(char * message) = 0;
--						~message: message content
--
-- RETURNS: void
--
-- NOTES: Sends a message to all the connected clients
--------------------------------------------------------------------------------------------------------------------*/
void ServerUDP::Broadcast(char * message)
{

}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	Send
--
-- DATE:		Febuary 28th, 2016		REVISIONS:
--
-- DESIGNER:	Ruoqi Jia				PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	void Broadcast(LPSOCKET_INFORMATION SocketInfo, char * message);
--                      ~SocketInfo : Pointer to Socket Information structure
--						~message    : message to send
--
-- RETURNS: void
--
-- NOTES: Sends a message to a specific connected client
--------------------------------------------------------------------------------------------------------------------*/
void ServerUDP::Send(LPSOCKET_INFORMATION SocketInfo, char * message)
{

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
-- NOTES: Callback completion routine for recvfrom when a packet has been recieved.
--------------------------------------------------------------------------------------------------------------------*/
void ServerUDP::RoutineManager(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags)
{

}
