#ifndef CLIENT_H
#define CLIENT_H
#include "globals.h"

class Client
{
    public:
        SOCKET_INFORMATION      SocketInfo;

        Client(){}
        ~Client(){}

        bool InitializeSocket(short port);

        virtual bool Recv() = 0;

        bool Send(char * message, int size);

        virtual bool Close() = 0;


    protected:
        SOCKADDR_IN             LocalAddr;    // Server address structures
        WSADATA                 wsaData;            // Session info
        CircularBuffer          CBPackets; // buffer for data coming in from network

};

#endif // CLIENT_H
