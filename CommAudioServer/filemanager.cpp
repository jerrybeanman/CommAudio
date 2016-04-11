#include "filemanager.h"
#include <sstream>

FILE * fp;
ServerTCP serverTCP;

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

DWORD WINAPI TCPReadThread(LPVOID lpParamater)
{
    Client * client = (Client *)lpParamater;
    while((client->SocketInfo.BytesRECV = recv(client->SocketInfo.Socket, client->SocketInfo.DataBuf.buf, DATA_BUFSIZE, 0)) != SOCKET_ERROR)
	{
        ParseRequestMessage(&client->SocketInfo);
	}
	return TRUE;
}

void ParseRequestMessage(LPSOCKET_INFORMATION SocketInfo)
{
    std::string buf(SocketInfo->DataBuf.buf);
	std::stringstream iss(buf);
	int RequestVal;
	iss >> RequestVal;
	switch(RequestVal)
	{
        case ClientJoined :
		{
			std::string packet = BuildFilePacket();
            SocketInfo->DataBuf.buf = (char*)packet.c_str();
            SocketInfo->DataBuf.len = packet.length() + 1;
			serverTCP.Send(SocketInfo, (char*)packet.c_str());
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
	        // read all (real) files in current folder
	        // , delete '!' read other 2 default folder . and ..
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
-- DATE:		Febuary 6th, 2016
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
        std::cerr << "Errno: " << errno << std::endl;
		return false;
	}
	return true;
}

void SendStartMessage(LPSOCKET_INFORMATION SocketInfo)
{
    std::string fileStart = FileBegin;
    SocketInfo->DataBuf.buf = (char *)fileStart.c_str();
    SocketInfo->DataBuf.len = fileStart.length() + 1;
    serverTCP.Send(SocketInfo, (char *)fileStart.c_str());
}

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
}

void SendEndMessage(LPSOCKET_INFORMATION SocketInfo)
{
    std::string fileEnd = FileEnd;
    SocketInfo->DataBuf.buf = (char *)fileEnd.c_str();
    SocketInfo->DataBuf.len = fileEnd.length() + 1;
    serverTCP.Send(SocketInfo, (char *)fileEnd.c_str());
}

std::string BuildFilePacket()
{
	std::ostringstream convert;   // stream used for the conversion
    convert << UpdateList;
	std::string packet = convert.str();
	for(auto &x : FileNames)
	{
		packet += " ";
		packet += x;
	}
	return packet;
}
