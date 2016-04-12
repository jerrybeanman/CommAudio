#ifndef CLIENTUDP_H
#define CLIENTUDP_H
#include "Client.h"
#include <ws2tcpip.h>

class ClientUDP : public Client
{
    public:
        bool InitializeSocket(short port);

        bool MulticastSettings(const char * name);

        bool Close();

        bool Recv();

        bool Send(char * message, int size);

        char* GetData();

        bool leaveMulticast();

        bool InitializeSendingSocket(char * ip, short port);
    private:
        struct ip_mreq      MulticastInfo;
        SOCKADDR_IN         SourceAddress;
        DWORD               Flags = 0;
        DWORD               SourceLen = sizeof(SourceAddress);
};

#endif // CLIENTUDP_H
