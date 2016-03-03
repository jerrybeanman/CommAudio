#include "ServerUDP.h"
int ServerUDP::InitializeSocket(short port)
bool ServerUDP::InitializeSocket()
{
    // Create a WSA v2.2 session
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed with error %d\n", WSAGetLastError());
        return false;
    }

    // Create socket for listening
    if ((ServerSocket = WSASocket(AF_INET,SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED) == INVALID_SOCKET))
    {
        printf("WSASocket() failed with error %d\n", WSAGetLastError());
        return false;
    }

    // Initialize address structure
    InternetAddr.sin_family         = AF_INET;
    InternetAddr.sin_addr.s_addr    = htonl(INADDR_ANY);
    InternetAddr.sin_port           = htons(DEFAULT_PORT);

    // Bind address to the listening socket
    if (bind(ServerSocket, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)
    {
        printf("bind() failed with error %d\n", WSAGetLastError());
        return false;
    }
    return true;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	InitializeSocket
--
-- DATE:		Febuary 28th, 2016          REVISIONS:
--
-- DESIGNER:	Ruoqi Jia, Scott Plummer	PROGRAMMER:	Ruoqi Jia, Scott Plummer
--
-- INTERFACE:	virtual int MulticastSettings(short port) = 0;
--
-- RETURNS: void
--
-- NOTES: Set time to live, multicast address, and disabled loop back
--------------------------------------------------------------------------------------------------------------------*/
bool ServerUDP::MulticastSettings()
{
    BOOL LoopBackFlag = false;
    //TODO replace with local ip address
    MulticastAddress.imr_multiaddr.s_addr = inet_addr(DEFAULT_IP);
    MulticastAddress.imr_interface.s_addr = INADDR_ANY;


    if(setsockopt(ServerSocket, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&TimeToLive, sizeof(TimeToLive)) == SOCKET_ERROR)
    {
        printf("setsockopt() failed with error on time to live%d\n", WSAGetLastError());
        return false;
    }

    if(setsockopt(ServerSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&MulticastAddress, sizeof(MulticastAddress)) == SOCKET_ERROR)
    {
        printf("setsockopt() failed with error on multicast address%d\n", WSAGetLastError());
        return false;
    }

    if(setsockopt(ServerSocket, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&LoopBackFlag, sizeof(LoopBackFlag)) == SOCKET_ERROR)
    {
        printf("Setsocketopt() failed with error on loop back%d\n", WSAGetLastError());
        return false;
    }

    DestinationAddress.sin_family       =      AF_INET;
    //TODO replace with local ip address
    DestinationAddress.sin_addr.s_addr  = inet_addr(DEFAULT_IP);
    DestinationAddress.sin_port         =        htons(DEFAULT_PORT);

    return true;
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
