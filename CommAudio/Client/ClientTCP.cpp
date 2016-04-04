#include "ClientTCP.h"
bool ClientTCP::InitializeSocket(short port, char *ip)
{
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cout << "WSAStartup failed with error " << WSAGetLastError() << std::endl;
        return FALSE;
    }

    if ((listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
    {
        std::cout << "WSASocket() failed with error " <<  WSAGetLastError() << std::endl;
        return FALSE;
    }
    bool fFlag = true;
    if(setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&fFlag, sizeof(fFlag)) == SOCKET_ERROR)
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

    if(bind(listenSocket, (struct sockaddr*) &LocalAddr, sizeof(LocalAddr)) == SOCKET_ERROR)
    {
        std::cout << "bind() failed with error " << WSAGetLastError() << std::endl;
        return FALSE;
    }

    if (listen(listenSocket, 1)) {
        return FALSE;
    }

    return TRUE;

}
void ClientTCP::acceptConnection(){
    SocketInfo.Socket = accept(listenSocket, NULL, NULL);
}

bool ClientTCP::Recv() {
    memset(SocketInfo.Buffer, '\0', DATA_BUFSIZE);
    SocketInfo.DataBuf.len = DATA_BUFSIZE;
    SocketInfo.DataBuf.buf = SocketInfo.Buffer;
    ZeroMemory(&SocketInfo.Overlapped, sizeof(WSAOVERLAPPED));
    SocketInfo.Overlapped.hEvent =  WSACreateEvent();
    if (WSARecv(
                SocketInfo.Socket,                  /* Accepted socket					*/
                &SocketInfo.DataBuf,				/* Message buffer to recieve		*/
                1,									/* Maximum data to recieve			*/
                NULL,                                /* No modification					*/
                &Flags,
                &SocketInfo.Overlapped,
                NULL)
                == SOCKET_ERROR) {
        if (WSAGetLastError() == WSA_IO_PENDING)
        {
            if (WSAWaitForMultipleEvents(1, &SocketInfo.Overlapped.hEvent, FALSE, INFINITE, FALSE) == WAIT_TIMEOUT)
            {
                std::cout << "RecvFrom() Timeout" << std::endl;
                return FALSE;
            }
        } else {
            std::cout << "RecvFrom() failed with error " << WSAGetLastError() << std::endl;
            return FALSE;
        }
    }

    if(!WSAGetOverlappedResult(SocketInfo.Socket, &(SocketInfo.Overlapped), &SocketInfo.BytesRECV, FALSE, &Flags))
    {
        std::cout << "ClientUDP::WSAGetOVerlappedResult failed with errno " << WSAGetLastError() << std::endl;
        return FALSE;
    }
    return TRUE;
}

bool ClientTCP::Send(char * message, int size) {
    Q_UNUSED(message)
    Q_UNUSED(size)

    return FALSE;
}


bool ClientTCP::Close() {
    if(closesocket(SocketInfo.Socket) == SOCKET_ERROR) {
        return false;
    }
    WSACleanup();

    return true;
}
