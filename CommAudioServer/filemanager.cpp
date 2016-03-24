#include "filemanager.h"
#include <sstream>

ServerTCP serverTCP;

bool StartFileManager()
{
    DWORD TCPServerThreadID;

    if(serverTCP.InitializeSocket(DEFAULT_PORT) < 0)
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
	while((client->SocketInfo.DataBuf.len = recv(client->SocketInfo.Socket, client->SocketInfo.DataBuf.buf, DATA_BUFSIZE, 0)) != SOCKET_ERROR)
	{
        ParseRequestMessage(client->SocketInfo.DataBuf.buf);
	}
	return TRUE;
}

void ParseRequestMessage(char * message)
{
	std::string buf(message);
	std::stringstream iss(buf);
	int RequestVal;
	iss >> RequestVal;
	switch(RequestVal)
	{
		case ClientJoined:

		break;
		case FileRequest:
			std::string filename;
			iss >> filename;
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
