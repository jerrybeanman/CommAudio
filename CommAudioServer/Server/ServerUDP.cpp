#include "ServerUDP.h"
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
    if ((ServerSocket = socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
    {
        std::cout << "WSASocket() failed with error " << WSAGetLastError() << std::endl;
        return false;
    }

    if (setsockopt(ServerSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt)) < 0)
    {
        std::cout << "WSASocket() failed with error " << WSAGetLastError() << std::endl;
        return false;
    }
    // Initialize address structure
    InternetAddr.sin_family         = AF_INET;
    InternetAddr.sin_addr.s_addr    = htonl(INADDR_ANY);
    InternetAddr.sin_port           = htons(port);

    // Bind address to the listening socket
    if (bind(ServerSocket, (LPSOCKADDR)&InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)
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


    if(setsockopt(ServerSocket, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&TimeToLive, sizeof(TimeToLive)) == SOCKET_ERROR)
    {
        std::cout << "setsockopt() failed with error on time to live" << WSAGetLastError() << std::endl;
        return false;
    }

    if(setsockopt(ServerSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&MulticastAddress, sizeof(MulticastAddress)) == SOCKET_ERROR)
    {
        std::cout << "setsockopt() failed with error on multicast address " <<  WSAGetLastError() << std::endl;
        return false;
    }

    if(setsockopt(ServerSocket, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&LoopBackFlag, sizeof(LoopBackFlag)) == SOCKET_ERROR)
    {
        std::cout << "Setsocketopt() failed with error on loop back" << WSAGetLastError() << std::endl;
        return false;
    }

    DestinationAddress.sin_family       = AF_INET;
    DestinationAddress.sin_addr.s_addr  = inet_addr(name);
    DestinationAddress.sin_port         = htons(DEFAULT_PORT);

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
bool ServerUDP::Broadcast(char * message)
{

    /* Send the time to our multicast group! */
    if(sendto(ServerSocket, message, sizeof(message), 0, (struct sockaddr*)&DestinationAddress,sizeof(DestinationAddress)) < 0)
    {
        std::cout << "ServerUDP::sendTo() failed with error" << WSAGetLastError() << std::endl;
        return false;
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

