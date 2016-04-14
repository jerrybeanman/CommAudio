#ifndef TCP_SERVER_H
#define TCP_SERVER_H
#include <map>
#include "Server.h"

/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE:	ServerTCP.h - Contains logical functionalities for TCP control channel connections
--
-- PROGRAM:		Comm Audio
--
-- FUNCTIONS:	bool InitializeSocket(short port);
--              std::string Accept(void);
--              bool Broadcast(char * message, DWORD lpNumberOfBytesSent);
--              void Send(LPSOCKET_INFORMATION sockinfo, char * message);
--              void RoutineManager(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags);
--              void AssignID(Client client);
-- DATE:		April 12th, 2016
--
-- REVISIONS:
--
-- DESIGNER:	Ruoqi Jia
--
-- PROGRAMMER:	Ruoqi Jia
--------------------------------------------------------------------------------------------------------------------*/
struct Client
{
      SOCKET_INFORMATION SocketInfo;
      SOCKADDR_IN        Connection;
};
class ServerTCP : public Server
{

        public:
            std::map<std::string, Client> ClientList;

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
            bool InitializeSocket(short port);

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
            std::string Accept(void);

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
            bool Broadcast(char * message, DWORD lpNumberOfBytesSent);

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
            void Send(LPSOCKET_INFORMATION sockinfo, char * message);

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

             void AssignID(Client client);
        private:
            SOCKET      AcceptedSocket;
    };
#endif // TCP_SERVER_H
