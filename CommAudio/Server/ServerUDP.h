#ifndef SERVERUDP_H
#define SERVERUDP_H
#include "Server.h"
#include <ws2tcpip.h>

class ServerUDP : public Server
{

        public:

            ServerUDP(){}
            ~ServerUDP(){}
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
            bool InitializeSocket(short port);

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
            bool MulticastSettings(const char * name);

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
            bool Broadcast(char * message);


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
            void Send(LPSOCKET_INFORMATION SocketInfo, char * message);

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
            void RoutineManager(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags);

        private:
            struct ip_mreq   MulticastAddress;
            SOCKADDR_IN      DestinationAddress;
            
    };
#endif // SERVERUDP_H
