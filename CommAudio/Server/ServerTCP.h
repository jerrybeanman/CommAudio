#ifndef TCP_SERVER_H
#define TCP_SERVER_H
#include "Server.h"

class ServerTCP : public Server
    {
        public:

            ServerTCP(){}
            ~ServerTCP(){}
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
            int InitializeSocket();

            /*------------------------------------------------------------------------------------------------------------------
            -- FUNCTION:	Accept
            --
            -- DATE:		Febuary 28th, 2016		REVISIONS:
            --
            -- DESIGNER:	Ruoqi Jia				PROGRAMMER:	Ruoqi Jia
            --
            -- INTERFACE:	int Accept(void);
            --
            -- RETURNS: void
            --
            -- NOTES:  Calls accept on a player's socket. Sets the returning socket and client address structure to the player.
                Add connected player to the list of players
            --------------------------------------------------------------------------------------------------------------------*/
            int Accept(void);

            /*------------------------------------------------------------------------------------------------------------------
            -- FUNCTION:	Broadcast
            --
            -- DATE:		Febuary 28th, 2016		REVISIONS:
            --
            -- DESIGNER:	Ruoqi Jia				PROGRAMMER:	Ruoqi Jia
            --
            -- INTERFACE:	virtual void Broadcast(LPSOCKET_INFORMATION SocketInfor, char * message) = 0;
            --                      ~SocketInfo : Pointer to Socket Information structure
            --						~message    : message content
            --
            -- RETURNS: void
            --
            -- NOTES: Sends a message to all the connected clients
            --------------------------------------------------------------------------------------------------------------------*/
            void Broadcast(LPSOCKET_INFORMATION SocketInfo, char * message);

            /*------------------------------------------------------------------------------------------------------------------
            -- FUNCTION:	Send
            --
            -- DATE:		Febuary 28th, 2016		REVISIONS:
            --
            -- DESIGNER:	Ruoqi Jia				PROGRAMMER:	Ruoqi Jia
            --
            -- INTERFACE:	virtual void Send(LPSOCKET_INFORMATION sockinfo) = 0;
            --						~sockinfo: Pointer to the socket information structure
            --
            -- RETURNS: void
            --
            -- NOTES: Sends a message to a specific connected client
            --------------------------------------------------------------------------------------------------------------------*/
            void Send(LPSOCKET_INFORMATION sockinfo);

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
             void RoutineManager(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags);

        private:
            SOCKET  AcceptedSocket;
    };
#endif // TCP_SERVER_H
