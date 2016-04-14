#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include "globals.h"
#include "Audio/datagenerator.h"
#include "Server/ServerUDP.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	StartSoundManagers
--
-- DATE:		April 11th, 2016
--
-- REVISIONS:
--
-- DESIGNER:	Ruoqi Jia
--
-- PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	bool StartSoundManager()
--
-- RETURNS: True on succesfull execution on UDP socket, false otherwise
--
-- NOTES: Initialize socket and creates the accepting thread
--------------------------------------------------------------------------------------------------------------------*/
bool StartSoundManager();

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	BroadcastMusic
--
-- DATE:		April 11th, 2016
--
-- REVISIONS:
--
-- DESIGNER:	Ruoqi Jia
--
-- PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	DWORD WINAPI BroadcastMusic(LPVOID lpParameter)
--
-- RETURNS: Thread exit code
--
-- NOTES: Initialize thread for broacasting music data to the multicast channel

--------------------------------------------------------------------------------------------------------------------*/
DWORD WINAPI BroadcastMusic(LPVOID lpParameter);

#endif // SOUNDMANAGER_H
