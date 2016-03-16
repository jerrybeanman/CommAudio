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
std::vector<std::string> get_all_files_names_within_folder(std::wstring folder);
#endif // FILEMANAGER_H
