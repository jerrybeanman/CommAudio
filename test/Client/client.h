#ifndef CLIENT_H
#define CLIENT_H
#include "circularbuffer.h"
#include "globals.h"

class Client
{
    public:
        SOCKET_INFORMATION      SocketInfo;

        Client(){}
        ~Client(){}

        virtual bool InitializeSocket(short port) = 0;

        virtual bool Recv() = 0;

        virtual bool Send(char * message, int size) = 0;


    protected:
        SOCKADDR_IN             LocalAddr;    // Server address structures
        WSADATA                 wsaData;            // Session info
        CircularBuffer          CBPackets; // buffer for data coming in from network

};

#endif // CLIENT_H
