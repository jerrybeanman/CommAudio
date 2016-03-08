#ifndef CLIENTUDP_H
#define CLIENTUDP_H
#include "Client.h"

class ClientUDP : public Client
{
    public:
        ClientUDP();
        ~ClientUDP();

        bool InitializeSocket(const char* name, short port);

        void * Recv() = 0;

        bool Send(char * message, int size) = 0;

        char* GetData();
};

#endif // CLIENTUDP_H
