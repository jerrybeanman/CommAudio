#include "filemanager.h"
#include <sstream>

FILE * fp;
ServerTCP serverTCP;

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
bool StartFileManager()
{
    DWORD TCPServerThreadID;

    if(serverTCP.InitializeSocket(DEFAULT_PORT_TCP) < 0)
        return FALSE;

	if(CreateThread(NULL, 0, TCPAccept, (LPVOID)&serverTCP, 0, &TCPServerThreadID) == NULL)
    {
        std::cout << "StartSoundManager()::Thread creation for BroadcastMusic() failed " << std::endl;
    	return FALSE;
    }
	return TRUE;
}

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
DWORD WINAPI TCPAccept(LPVOID lpParameter)
{
	DWORD ReadThreadID;
	while(1)
	{
		std::string ip;
        if((ip = serverTCP.Accept()) != "")
		{
			if(CreateThread(NULL, 0, TCPReadThread, (LPVOID)&serverTCP.ClientList[ip], 0, &ReadThreadID) == NULL)
		    {
                std::cout << "StartSoundManager()::Thread creation for BroadcastMusic() failed " << std::endl;
		    	return FALSE;
		    }
		}
	}
	return TRUE;
}

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
DWORD WINAPI TCPReadThread(LPVOID lpParamater)
{
    Client * client = (Client *)lpParamater;
    while((client->SocketInfo.BytesRECV = recv(client->SocketInfo.Socket, client->SocketInfo.DataBuf.buf, DATA_BUFSIZE, 0)) != SOCKET_ERROR)
	{
        ParseRequestMessage(&client->SocketInfo);
	}
	return TRUE;
}

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
void ParseRequestMessage(LPSOCKET_INFORMATION SocketInfo)
{
    std::string buf(SocketInfo->DataBuf.buf);
	std::stringstream iss(buf);
	int RequestVal;
	iss >> RequestVal;
	switch(RequestVal)
	{
		case ClientJoined:
        {
            SendSongHeader(SocketInfo);
            SendSongList(SocketInfo);
		}
		break;
        case ListRequest:
        {
            std::string packet = UpdateList;
            for(auto &x : FileNames)
            {
                packet.append("@");
                packet.append(x);
            }
            SocketInfo->DataBuf.buf = (char*)packet.c_str();
            SocketInfo->DataBuf.len = packet.length() + 1;
            serverTCP.Send(SocketInfo, (char*)packet.c_str() + 1);
        }
        break;
		case FileRequest:
		{
			std::string filename;
			iss >> filename;
            if(OpenFile(filename))
			{
                SendStartMessage(SocketInfo);
                SendFile(SocketInfo, fp);
                SendEndMessage(SocketInfo);
			}
		}
		break;
	}
}
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
std::vector<std::string> GetFileNames(std::string folder, std::string extension)
{
    std::wstring path(folder.begin(), folder.end());
    std::wstring tmpExtension(extension.begin(), extension.end());
    path += L"\\*.";
    path += tmpExtension;
    path += L"*";
    std::vector<std::string> names;
    WIN32_FIND_DATA fd;
    HANDLE hFind = FindFirstFile(path.c_str(), &fd);
    if(hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
	        {
                std::wstring tmp(fd.cFileName);
                names.push_back(std::string(tmp.begin(), tmp.end()));
	        }
        }while(FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }
    return names;
}


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
bool OpenFile(std::string name)
{
    std::string dir = MUSIC_DIRECTORY + name;
    if ((fp = fopen(dir.c_str(), "rb")) == NULL)
	{
        std::cerr << "File open errno: " << errno << std::endl;
		return false;
	}
	return true;
}

void SendSongList(LPSOCKET_INFORMATION SocketInfo)
{
    std::string packet = BuildFilePacket();
    SocketInfo->DataBuf.buf = (char*)packet.c_str();
    SocketInfo->DataBuf.len = packet.length() + 1;
    serverTCP.Send(SocketInfo, (char*)packet.c_str() + 1);
}

void SendSongName(LPSOCKET_INFORMATION SocketInfo)
{
    std::string name = SongName + Currentsong;
    SocketInfo->DataBuf.buf = (char *)name.c_str();
    SocketInfo->DataBuf.len = name.length() + 1;
    serverTCP.Send(SocketInfo, (char *)name.c_str() + 1);
}

void SendSongHeader(LPSOCKET_INFORMATION SocketInfo)
{
    std::string header = Header + SongHeader;
    SocketInfo->DataBuf.buf = (char *)header.c_str();
    SocketInfo->DataBuf.len = header.length() + 1;
    serverTCP.Send(SocketInfo, (char *)header.c_str() + 1);
}

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
void SendStartMessage(LPSOCKET_INFORMATION SocketInfo)
{
    std::string fileStart = FileBegin;
    SocketInfo->DataBuf.buf = (char *)fileStart.c_str();
    SocketInfo->DataBuf.len = fileStart.length() + 1;
    serverTCP.Send(SocketInfo, (char *)fileStart.c_str());
}

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
void SendFile(LPSOCKET_INFORMATION SocketInfo, FILE * fp)
{
	DWORD		FBytesRead;		/* Bytes read from fread						*/
    do
    {
        char * pbuf = (char *)malloc(DATA_BUFSIZE);
        FBytesRead = fread(pbuf, 1, DATA_BUFSIZE, fp);
        SocketInfo->DataBuf.buf = pbuf;
        SocketInfo->DataBuf.len = FBytesRead;
        serverTCP.Send(SocketInfo, pbuf);

        /* zero out memory for next round */
        free(pbuf);
    }while(FBytesRead == DATA_BUFSIZE);
    fclose(fp);
}

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
void SendEndMessage(LPSOCKET_INFORMATION SocketInfo)
{
    std::string fileEnd = FileEnd;
    SocketInfo->DataBuf.buf = (char *)fileEnd.c_str();
    SocketInfo->DataBuf.len = fileEnd.length() + 1;
    serverTCP.Send(SocketInfo, (char *)fileEnd.c_str());
}

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
std::string BuildFilePacket()
{
	std::ostringstream convert;   // stream used for the conversion
    convert << UpdateList;
    std::string packet = convert.str();
	for(auto &x : FileNames)
	{
        packet.append("@");
        packet.append(x);
	}
    packet.append("%");
    packet.append(SongName);
    packet.append(Currentsong);
	return packet;
}
