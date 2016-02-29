#ifndef CLIENT_H
#define CLIENT_H
#include "globals.h"

class Client
{
    public:

        Client();
        ~Client();

        virtual int InitializeSocket(const char* name, short port) = 0;

        virtual void * Recv() = 0;

        virtual int Send(char * message, int size) = 0;

        int Init_SockAddr(const char* hostname, short hostport);

        char* GetData();

    protected:
        CircularBuffer  CBPackets; // buffer for data coming in from network
        LPSOCKET_INFORMATION SockInfo;

};

#endif // CLIENT_H
