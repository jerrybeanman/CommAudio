#ifndef SERVER_H
#define SERVER_H
#include "globals.h"

class Server
    {
        public:
            Server(){}
            ~Server(){}
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
            virtual int InitializeSocket(short port) = 0;

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
            virtual void Broadcast(char * message) = 0;

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
            virtual void Send(LPSOCKET_INFORMATION sockinfo) = 0;

            /*------------------------------------------------------------------------------------------------------------------
            -- FUNCTION:	RoutineManager
            --
            -- DATE:		Febuary 28th, 2016		REVISIONS:
            --
            -- DESIGNER:	Ruoqi Jia				PROGRAMMER:	Ruoqi Jia
            --
            -- INTERFACE:	virtual void RoutineManager(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags) = 0;
            --                      ~Error				: Error code
            --                      ~BytesTransffered	: Total bytes recieved from packet
            --                      ~Overlapped			: Overlapped structure
            --                      ~InFlags            : Modification flags
            -- RETURNS: void
            --
            -- NOTES: Callback completion routine for recv when a packet has been recieved.
            --------------------------------------------------------------------------------------------------------------------*/
            virtual void RoutineManager(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags) = 0;

        private:

            /* Circular buffer for server data processing */
            CircularBuffer CircularBuff;
    };

#endif // SERVER_H
