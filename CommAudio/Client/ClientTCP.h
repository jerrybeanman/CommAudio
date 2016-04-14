#ifndef CLIENTTCP_H
#define CLIENTTCP_H
#include "Client.h"

/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE:	ClientUDP.h - provides logical function for TCP connection
--
-- PROGRAM:		Comm Audio
--
-- FUNCTIONS:   bool InitializeSocket(short port, char *ip);
--              bool Close();
--              bool Recv();
--              bool Send(char * message, int size);
--              char* GetData();
--
-- DATE:		April 12th, 2016
--
-- REVISIONS:
--
-- DESIGNER:	Ruoqi Jia
--
-- PROGRAMMER:	Ruoqi Jia
--------------------------------------------------------------------------------------------------------------------*/
class ClientTCP : public Client
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
    -- INTERFACE:	bool ClientTCP::InitializeSocket(short port, char *ip)
    --
    -- RETURNS: true on success, false otherwise
    --
    -- NOTES: Initialize socket to a specific address
    --------------------------------------------------------------------------------------------------------------------*/
    bool InitializeSocket(short port, char *ip);
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Close
    --
    -- DATE:		April 11th, 2016
    --
    -- REVISIONS:
    --
    -- DESIGNER:	Ruoqi Jia
    --
    -- PROGRAMMER:	Ruoqi Jia
    --
    -- INTERFACE:	bool ClientTCP::Close()
    --
    -- RETURNS: true on success, false otherwise
    --
    -- NOTES: close socket
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
    -- INTERFACE:	bool ClientTCP::Recv()
    --
    -- RETURNS: true on success, false otherwise
    --
    -- NOTES: recieve from the TCP socket
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
    -- INTERFACE:	bool ClientTCP::Send(char * message, int size)
    --
    -- RETURNS: true on success, false otherwise
    --
    -- NOTES: Send message to the TCP socket
    --------------------------------------------------------------------------------------------------------------------*/
    bool Send(char * message, int size);

    char* GetData();

    SOCKET              listenSocket;
    WSAEVENT            WSAEvent;
private:
    SOCKADDR_IN         SourceAddress;
    DWORD               Flags = 0;
    DWORD               SourceLen = sizeof(SourceAddress);

};

#endif // CLIENTTCP_H
