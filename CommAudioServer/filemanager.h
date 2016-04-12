#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <vector>
#include <string>
#include "Server/ServerTCP.h"
#include "globals.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	StartFileManager
--
-- DATE:		April 11th, 2016
--
-- REVISIONS:
--
-- DESIGNER:	Ruoqi Jia
--
-- PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	bool StartFileManager()
--
-- RETURNS: True on succesfull execution on socket, false otherwise
--
-- NOTES: Initialize socket and creates the accepting thread
--------------------------------------------------------------------------------------------------------------------*/
bool StartFileManager();

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	TCPAccept
--
-- DATE:		April 11th, 2016
--
-- REVISIONS:
--
-- DESIGNER:	Ruoqi Jia
--
-- PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	DWORD WINAPI TCPAccept(LPVOID lpParameter)
--                  lpParameter : Expected socket to pass in
--
-- RETURNS: thread exit code
--
-- NOTES: background thread to listen and accept connections. When a client is connected it creates a thread to manage
--          incoming data on that specific socket
--------------------------------------------------------------------------------------------------------------------*/
DWORD WINAPI TCPAccept(LPVOID lpParameter);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	TCPReadThread
--
-- DATE:		April 11th, 2016
--
-- REVISIONS:
--
-- DESIGNER:	Ruoqi Jia
--
-- PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	DWORD WINAPI TCPReadThread(LPVOID lpParamater)
--                  lpParameter : Expected socket passed in
--
-- RETURNS: thread exit code
--
-- NOTES: background thread to recieve data on a TCP socket
--------------------------------------------------------------------------------------------------------------------*/
DWORD WINAPI TCPReadThread(LPVOID lpParameter);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	ParseRequestMessage
--
-- DATE:		April 11th, 2016
--
-- REVISIONS:
--
-- DESIGNER:	Ruoqi Jia
--
-- PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	void ParseRequestMessage(LPSOCKET_INFORMATION SocketInfo)
--                  SocketInfo  : Socket to send to
--
-- RETURNS: void
--
-- NOTES: Determine the message type recieved from client
--------------------------------------------------------------------------------------------------------------------*/
void ParseRequestMessage(LPSOCKET_INFORMATION SocketInfo);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	GetFileNames
--
-- DATE:		April 11th, 2016
--
-- REVISIONS:
--
-- DESIGNER:	Ruoqi Jia
--
-- PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	std::vector<std::string> GetFileNames(std::string folder, std::string extension)
--                  folder      : Music folder directory
--                  extension   : Extension of the files to obtain
--
-- RETURNS: vector containing the name of each file
--
-- NOTES: retrieves a vector containing the name of each file with the specific extension in a target directory
--------------------------------------------------------------------------------------------------------------------*/
std::vector<std::string> GetFileNames(std::string folder, std::string extension);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	OpenFile
--
-- DATE:		April 11th, 2016
--
-- REVISIONS:
--
-- DESIGNER:	Ruoqi Jia
--
-- PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	int OpenFile()
--
-- RETURNS: success value
--
-- NOTES: Wrapper function to open a file. File name is specified in the UI
--------------------------------------------------------------------------------------------------------------------*/
bool OpenFile(std::string name);

void SendSongList(LPSOCKET_INFORMATION SocketInfo);
void SendSongHeader(LPSOCKET_INFORMATION SocketInfo);
void SendSongName(LPSOCKET_INFORMATION SocketInfo);
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	SendStartMessage
--
-- DATE:		April 11th, 2016
--
-- REVISIONS:
--
-- DESIGNER:	Ruoqi Jia
--
-- PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	void SendStartMessage(LPSOCKET_INFORMATION SocketInfo)
--                  SocketInfo: Socket to send to
--
-- RETURNS: void
--
-- NOTES: Sends a TCp message to the specific client indicating a start of file transfer packet
--------------------------------------------------------------------------------------------------------------------*/
void SendStartMessage(LPSOCKET_INFORMATION SocketInfo);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	SendFile
--
-- DATE:		April 11th, 2016
--
-- REVISIONS:
--
-- DESIGNER:	Ruoqi Jia
--
-- PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	void SendFile(LPSOCKET_INFORMATION SocketInfo, FILE * fp)
--                  SocketInfo  : Socket to send to
--                  fp          : File pointer
--
-- RETURNS: void
--
-- NOTES: Send a file to the specific client through TCp
--------------------------------------------------------------------------------------------------------------------*/
void SendFile(LPSOCKET_INFORMATION SockInfo, FILE * fp);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	SendEndMessage
--
-- DATE:		April 11th, 2016
--
-- REVISIONS:
--
-- DESIGNER:	Ruoqi Jia
--
-- PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	void SendEndMessage(LPSOCKET_INFORMATION SocketInfo)
--                  SocketInfo: Socket to send to
--
-- RETURNS: void
--
-- NOTES: Sends a TCp message to the specific client indicating the end of file transfer packet
--------------------------------------------------------------------------------------------------------------------*/
void SendEndMessage(LPSOCKET_INFORMATION SocketInfo);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	BuildFilePacket
--
-- DATE:		April 11th, 2016
--
-- REVISIONS:
--
-- DESIGNER:	Ruoqi Jia
--
-- PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	std::string BuildFilePacket()
--
-- RETURNS: string containing delimilited file names in the Music directory
--
-- NOTES: Builds a string from FileNames, which contains each file names in the Music directory delimited by space
--------------------------------------------------------------------------------------------------------------------*/
std::string BuildFilePacket();
#endif // FILEMANAGER_H
