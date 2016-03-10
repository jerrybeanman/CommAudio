#include "filemanager.h"
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
		/*(if(serverTCP.Accept())
		{
			if(CreateThread(NULL, 0, TCPReadThread, (LPVOID)&serverTCP, 0, &ReadThreadID) == NULL)
		    {
		    	std::cout << "StartSoundManager()::Thread creation for BroadcastMusic() failed " << st::endl;
		    	return FALSE;
		    }
		}*/
	}
	return TRUE;
}

DWORD WINAPI TCPReadThread(LPVOID lpParamater)
{
	return TRUE;
}
