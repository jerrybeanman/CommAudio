#ifndef CLIENTTCP_H
#define CLIENTTCP_H
#include "Client.h"

class ClientTCP : public Client
{
public:
    bool InitializeSocket(short port, char *ip);

    void Close();

    bool RecvFrom();

    bool SendTo(char * message, int size);

    char* GetData();

private:
    SOCKADDR_IN         SourceAddress;
    DWORD               Flags = 0;
    DWORD               SourceLen = sizeof(SourceAddress);
};

#endif // CLIENTTCP_H
