#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <vector>
#include "Server/ServerTCP.h"
#include "globals.h"
#include <string>

bool StartFileManager();

DWORD WINAPI TCPAccept(LPVOID lpParameter);
DWORD WINAPI TCPReadThread(LPVOID lpParameter);
void ParseRequestMessage(LPSOCKET_INFORMATION SocketInfo);
std::vector<std::string> GetFileNames(std::string folder, std::string extension);
bool OpenFile(std::string name);
void SendFile(LPSOCKET_INFORMATION SockInfo, FILE * fp);
std::string BuildFilePacket();
#endif // FILEMANAGER_H
