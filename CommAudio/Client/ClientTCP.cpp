#include "ClientTCP.h"

bool ClientTCP::InitializeSocket(short port, char *ip)
{
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cout << "WSAStartup failed with error " << WSAGetLastError() << std::endl;
        return FALSE;
    }

    if ((SocketInfo.Socket = WSASocket(AF_INET,SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
    {
        std::cout << "WSASocket() failed with error " <<  WSAGetLastError() << std::endl;
        return FALSE;
    }
    bool fFlag = true;
    if(setsockopt(SocketInfo.Socket, SOL_SOCKET, SO_REUSEADDR, (char *)&fFlag, sizeof(fFlag)) == SOCKET_ERROR)
    {
        std::cout << "InitializeSocket()::setsockopt() failed with error " << WSAGetLastError() << std::endl;
        return FALSE;
    }

    LocalAddr.sin_family = AF_INET;
    LocalAddr.sin_port = htons(port);

    struct hostent	*hp;
    if ((hp = gethostbyname(ip)) == NULL)
    {

        return FALSE;
    }

    memcpy((char *)&LocalAddr.sin_addr, hp->h_addr, hp->h_length);

    if(bind(SocketInfo.Socket, (struct sockaddr*) &LocalAddr, sizeof(LocalAddr)) == SOCKET_ERROR)
    {
        std::cout << "bind() failed with error " << WSAGetLastError() << std::endl;
        return FALSE;
    }

    return TRUE;

}

bool ClientTCP::RecvFrom() {

}

bool ClientTCP::SendTo(char * message, int size) {

}


void ClientTCP::Close() {
    closesocket(SocketInfo.Socket);
    WSACleanup();
}
