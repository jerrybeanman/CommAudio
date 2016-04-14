#include "ServerUDP.h"

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
-- NOTES: Initialize server udp socket
--------------------------------------------------------------------------------------------------------------------*/
bool ServerUDP::InitializeSocket(short port)
{
    int opt = 1;
    // Create a WSA v2.2 session
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cout << "WSAStartup failed with error" << WSAGetLastError() << std::endl;
        return false;
    }

    // Create socket for listening
    if ((SocketInfo.Socket = WSASocket(AF_INET,SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
    {
        std::cout << "WSASocket() failed with error " << WSAGetLastError() << std::endl;
        return false;
    }


    // Initialize address structure
    LocalAddr.sin_family         = AF_INET;
    LocalAddr.sin_addr.s_addr    = htonl(INADDR_ANY);
    LocalAddr.sin_port           = htons(port);

    // Bind address to the listening socket
    if (bind(SocketInfo.Socket, (LPSOCKADDR)&LocalAddr, sizeof(LocalAddr)) == SOCKET_ERROR)
    {
        std::cout << "bind() failed with error " << WSAGetLastError() << std::endl;
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
bool ServerUDP::MulticastSettings(const char * name)
{
    BOOL LoopBackFlag = false;

    //TODO replace with local ip address
    MulticastAddress.imr_multiaddr.s_addr = inet_addr(name);
    MulticastAddress.imr_interface.s_addr = INADDR_ANY;
    u_long                  aTimeToLive = 1;


    if(setsockopt(SocketInfo.Socket, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&aTimeToLive, sizeof(aTimeToLive)) == SOCKET_ERROR)
    {
        std::cout << "setsockopt() failed with error on time to live" << WSAGetLastError() << std::endl;
        return false;
    }

    if(setsockopt(SocketInfo.Socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&MulticastAddress, sizeof(MulticastAddress)) == SOCKET_ERROR)
    {
        std::cout << "setsockopt() failed with error on multicast address " <<  WSAGetLastError() << std::endl;
        return false;
    }

    if(setsockopt(SocketInfo.Socket, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&LoopBackFlag, sizeof(LoopBackFlag)) == SOCKET_ERROR)
    {
        std::cout << "Setsocketopt() failed with error on loop back" << WSAGetLastError() << std::endl;
        return false;
    }

    DestinationAddress.sin_family       = AF_INET;
    DestinationAddress.sin_addr.s_addr  = inet_addr(name);
    DestinationAddress.sin_port         = htons(DEFAULT_PORT);

    return true;
}

bool ServerUDP::Broadcast(char *message)
{

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
bool ServerUDP::Broadcast(char * message, DWORD lpNumberOfBytesSent)
{

    SocketInfo.DataBuf.buf = message;
    SocketInfo.DataBuf.len = lpNumberOfBytesSent;

    ZeroMemory(&SocketInfo.Overlapped, sizeof(WSAOVERLAPPED));
    SocketInfo.Overlapped.hEvent =  WSACreateEvent();

    if (WSASendTo(SocketInfo.Socket,      /* Writing socket                       */
            &(SocketInfo.DataBuf),        /* Message content                      */
            1,
            NULL,     /* Size of the message                  */
            flags,                              /* Bytes that are sent                  */
            (SOCKADDR *)&DestinationAddress,             /* Server socket address structure      */
            sizeof(DestinationAddress),
            &(SocketInfo.Overlapped),
            NULL)                       /* size of the socket address structure */
            < 0)
    {
        if (WSAGetLastError() != WSA_IO_PENDING)
        {
            std::cout << "ServerUDP::WSASendto() () failed with error " << WSAGetLastError() << std::endl;
            return FALSE;
        }
        if (WSAWaitForMultipleEvents(1, &SocketInfo.Overlapped.hEvent, FALSE, INFINITE, FALSE) == WAIT_TIMEOUT)
        {
            std::cout << "ServerUDP::WSASendto() Timeout" << std::endl;
            return FALSE;
        }
    }

    //Get the actual bytes sent.
    if(!WSAGetOverlappedResult(SocketInfo.Socket, &(SocketInfo.Overlapped), &SocketInfo.BytesSEND, FALSE, &flags))
    {
        std::cout << "SeverUDP::WSAGetOverlappedResult failed with errno" << WSAGetLastError() << std::endl;
        return FALSE;
    }
    //std::cerr << "ServerUDP::Broadcast>>Bytes Sent:[" << SocketInfo.BytesSEND << "]" << std::endl;
    return true;
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	Send
--
-- DATE:		Febuary 28th, 2016		REVISIONS:
--
-- DESIGNER:	Ruoqi Jia				PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	virtual void Send(LPSOCKET_INFORMATION sockinfo) = 0;
--						~sockinfo   : Pointer to the socket information structure
--                      ~message    : Message to send
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
