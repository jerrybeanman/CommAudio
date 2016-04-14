#ifndef CLIENTUDP_H
#define CLIENTUDP_H
#include "Client.h"
#include <ws2tcpip.h>

/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE:	ClientUDP.h - provides logical function for UDP connection
--
-- PROGRAM:		Comm Audio
--
-- FUNCTIONS:   bool InitializeSocket(short port);
--              bool MulticastSettings(const char * name);
--              bool Close();
--              bool Recv();
--              bool Send(char * message, int size);
--              char* GetData();
--              bool leaveMulticast();
--              bool InitializeSendingSocket(char * ip, short port);
--
-- DATE:		April 12th, 2016
--
-- REVISIONS:
--
-- DESIGNER:	Ruoqi Jia
--
-- PROGRAMMER:	Ruoqi Jia
--------------------------------------------------------------------------------------------------------------------*/
class ClientUDP : public Client
{
    public:
        /*------------------------------------------------------------------------------------------------------------------
        -- FUNCTION:	InitializeSocket
        --
        -- DATE:		April 11th, 2016
        --
        -- REVISIONS:
        --
        -- DESIGNER:	Ruoqi Jia
        --
        -- PROGRAMMER:	Ruoqi Jia
        --
        -- INTERFACE:	bool ClientUDP::InitializeSocket(short port)
        --
        -- RETURNS: void
        --
        -- NOTES: initialize a udp recieveing socket
        --------------------------------------------------------------------------------------------------------------------*/
        bool InitializeSocket(short port);

        /*------------------------------------------------------------------------------------------------------------------
        -- FUNCTION:	InitializeSendingSocket
        --
        -- DATE:		April 11th, 2016
        --
        -- REVISIONS:
        --
        -- DESIGNER:	Ruoqi Jia
        --
        -- PROGRAMMER:	Ruoqi Jia
        --
        -- INTERFACE:	bool ClientUDP::InitializeSendingSocket(char * ip, short port)
        --
        -- RETURNS: void
        --
        -- NOTES: initialize a udp sending socket to a specific address
        --------------------------------------------------------------------------------------------------------------------*/
        bool MulticastSettings(const char * name);

        /*------------------------------------------------------------------------------------------------------------------
        -- FUNCTION:	MulticastSettings
        --
        -- DATE:		April 11th, 2016
        --
        -- REVISIONS:
        --
        -- DESIGNER:	Ruoqi Jia
        --
        -- PROGRAMMER:	Ruoqi Jia
        --
        -- INTERFACE:	bool ClientUDP::MulticastSettings(const char * name)
        --
        -- RETURNS: void
        --
        -- NOTES: Set UDP recieving socket to a multicast channel
        --------------------------------------------------------------------------------------------------------------------*/
        bool Close();

        /*------------------------------------------------------------------------------------------------------------------
        -- FUNCTION:	Recv
        --
        -- DATE:		April 11th, 2016
        --
        -- REVISIONS:
        --
        -- DESIGNER:	Ruoqi Jia
        --
        -- PROGRAMMER:	Ruoqi Jia
        --
        -- INTERFACE:	bool ClientUDP::Recv()
        --
        -- RETURNS: true on success, false otherwise
        --
        -- NOTES: Recieves UDP datagram from socket
        --------------------------------------------------------------------------------------------------------------------*/
        bool Recv();

        /*------------------------------------------------------------------------------------------------------------------
        -- FUNCTION:	Send
        --
        -- DATE:		April 11th, 2016
        --
        -- REVISIONS:
        --
        -- DESIGNER:	Ruoqi Jia
        --
        -- PROGRAMMER:	Ruoqi Jia
        --
        -- INTERFACE:	bool ClientUDP::Send(char * message, int size)
        --
        -- RETURNS: true on success, false otherwise
        --
        -- NOTES: Send a message to the sending socket
        --------------------------------------------------------------------------------------------------------------------*/
        bool Send(char * message, int size);

        char* GetData();
        /*------------------------------------------------------------------------------------------------------------------
        -- FUNCTION:	leaveMulticast
        --
        -- DATE:		April 11th, 2016
        --
        -- REVISIONS:
        --
        -- DESIGNER:	Ruoqi Jia
        --
        -- PROGRAMMER:	Ruoqi Jia
        --
        -- INTERFACE:	bool ClientUDP::leaveMulticast()
        --
        -- RETURNS: true on success, false otherwise
        --
        -- NOTES: leave multicast group
        --------------------------------------------------------------------------------------------------------------------*/
        bool leaveMulticast();

        bool InitializeSendingSocket(char * ip, short port);
    private:
        struct ip_mreq      MulticastInfo;
        SOCKADDR_IN         SourceAddress;
        DWORD               Flags = 0;
        DWORD               SourceLen = sizeof(SourceAddress);
};

#endif // CLIENTUDP_H
