#ifndef CLIENT_H
#define CLIENT_H
#include "circularbuffer.h"
#include "globals.h"

class Client
{
    public:

        Client();
        ~Client();

        virtual bool InitializeSocket(const char* name, short port) = 0;

        virtual void * Recv() = 0;

        virtual bool Send(char * message, int size) = 0;

        bool Init_SockAddr(const char* hostname, short hostport);


    protected:
        SOCKADDR_IN             InternetAddr;    // Server address structures
        WSADATA                 wsaData;            // Session info
        CircularBuffer          CBPackets; // buffer for data coming in from network
        LPSOCKET_INFORMATION    SockInfo;

};

#endif // CLIENT_H
