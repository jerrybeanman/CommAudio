#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <vector>
#include "Server/ServerTCP.h"
#include "globals.h"
#include <string>

bool StartFileManager();

DWORD WINAPI TCPAccept(LPVOID lpParameter);
DWORD WINAPI TCPReadThread(LPVOID lpParameter);
void ParseRequestMessage(char * message);
std::vector<std::string> GetFileNames(std::string folder, std::string extension);
#endif // FILEMANAGER_H
