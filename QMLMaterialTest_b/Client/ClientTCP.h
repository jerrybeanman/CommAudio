#ifndef CLIENTTCP_H
#define CLIENTTCP_H
#include "Client.h"
class ClientTCP : public Client
{
public:
    bool InitializeSocket(short port, char *ip);

    bool Close();

    bool Recv();

    bool Send(char * message, int size);

    char* GetData();

    SOCKET              listenSocket;
    WSAEVENT            WSAEvent;
private:
    SOCKADDR_IN         SourceAddress;
    DWORD               Flags = 0;
    DWORD               SourceLen = sizeof(SourceAddress);
};

#endif // CLIENTTCP_H
