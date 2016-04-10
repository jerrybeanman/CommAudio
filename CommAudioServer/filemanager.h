#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <vector>
#include <string>
#include "Server/ServerTCP.h"
#include "globals.h"

bool StartFileManager();

DWORD WINAPI TCPAccept(LPVOID lpParameter);
DWORD WINAPI TCPReadThread(LPVOID lpParameter);
void ParseRequestMessage(LPSOCKET_INFORMATION SocketInfo);
std::vector<std::string> GetFileNames(std::string folder, std::string extension);
bool OpenFile(std::string name);
void SendStartMessage(LPSOCKET_INFORMATION SocketInfo);
void SendFile(LPSOCKET_INFORMATION SockInfo, FILE * fp);
void SendEndMessage(LPSOCKET_INFORMATION SocketInfo);
std::string BuildFilePacket();
#endif // FILEMANAGER_H
