#include "ClientUDP.h"

bool ClientUDP::InitializeSocket(short port)
{
    BOOL fFlag;
    // Create a WSA v2.2 session
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cout << "WSAStartup failed with error " << WSAGetLastError() << std::endl;
        return FALSE;
    }

    // Create socket for writing based on currently selected protocol
    if ((SocketInfo.Socket = WSASocket(AF_INET,SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
    {
        std::cout << "WSASocket() failed with error " <<  WSAGetLastError() << std::endl;
        return FALSE;
    }
    fFlag = TRUE;

    // Avoid WSAADDRINUSE erro on bind
    if(setsockopt(SocketInfo.Socket, SOL_SOCKET, SO_REUSEADDR, (char *)&fFlag, sizeof(fFlag)) == SOCKET_ERROR)
    {
        std::cout << "InitializeSocket()::setsockopt() failed with error " << WSAGetLastError() << std::endl;
        return FALSE;
    }

    // Assign the local port number to recieve on
    LocalAddr.sin_family		= AF_INET;
    LocalAddr.sin_addr.s_addr	= htonl(INADDR_ANY);
    LocalAddr.sin_port			= htons(port);

    // Bind local address to socket
    if(bind(SocketInfo.Socket, (struct sockaddr*) &LocalAddr, sizeof(LocalAddr)) == SOCKET_ERROR)
    {
        std::cout << "bind() failed with error " << WSAGetLastError() << std::endl;
        return FALSE;
    }
    return TRUE;
}

bool ClientUDP::InitializeSendingSocket(char * ip, short port)
{
    BOOL fFlag;
    // Create a WSA v2.2 session
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cout << "WSAStartup failed with error " << WSAGetLastError() << std::endl;
        return FALSE;
    }

    // Create socket for writing based on currently selected protocol
    if ((SocketInfo.Socket = WSASocket(AF_INET,SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
    {
        std::cout << "WSASocket() failed with error " <<  WSAGetLastError() << std::endl;
        return FALSE;
    }
    fFlag = TRUE;

    // Avoid WSAADDRINUSE erro on bind
    if(setsockopt(SocketInfo.Socket, SOL_SOCKET, SO_REUSEADDR, (char *)&fFlag, sizeof(fFlag)) == SOCKET_ERROR)
    {
        std::cout << "InitializeSocket()::setsockopt() failed with error " << WSAGetLastError() << std::endl;
        return FALSE;
    }

    // Assign the local port number to recieve on
    LocalAddr.sin_family		= AF_INET;
    LocalAddr.sin_port			= htons(port);
    struct hostent	*hp;
    if ((hp = gethostbyname(ip)) == NULL)
    {

        return FALSE;
    }

    memcpy((char *)&LocalAddr.sin_addr, hp->h_addr, hp->h_length);

    // Bind local address to socket
    /*if(bind(SocketInfo.Socket, (struct sockaddr*) &LocalAddr, sizeof(LocalAddr)) == SOCKET_ERROR)
    {
        std::cout << "bind() failed with error " << WSAGetLastError() << std::endl;
        return FALSE;
    }*/
    return TRUE;
}

bool ClientUDP::MulticastSettings(const char * name)
{
    MulticastInfo.imr_multiaddr.s_addr = inet_addr(name);
    MulticastInfo.imr_interface.s_addr = INADDR_ANY;

    // Join the multicast group
    if(setsockopt(SocketInfo.Socket, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                  (char *)&MulticastInfo, sizeof(struct ip_mreq)) == SOCKET_ERROR)
    {
        std::cout << "MulticastSettings::setsockopt() failed with error " << WSAGetLastError() << std::endl;
        return FALSE;
    }
    return TRUE;
}

bool ClientUDP::Recv()
{
    memset(SocketInfo.Buffer, '\0', DATA_BUFSIZE);
    SocketInfo.DataBuf.len = DATA_BUFSIZE;
    SocketInfo.DataBuf.buf = SocketInfo.Buffer;
    ZeroMemory(&SocketInfo.Overlapped, sizeof(WSAOVERLAPPED));
    SocketInfo.Overlapped.hEvent =  WSACreateEvent();
    if (WSARecvFrom(
                SocketInfo.Socket,                  /* Accepted socket					*/
                &SocketInfo.DataBuf,				/* Message buffer to recieve		*/
                1,									/* Maximum data to recieve			*/
                NULL,
                &Flags,                             /* No modification					*/
                (SOCKADDR *)&SourceAddress,			/* Server socket address structure	*/
                (LPINT)&SourceLen,
                &SocketInfo.Overlapped,
                NULL)
                == SOCKET_ERROR)
    {
        if (WSAGetLastError() == WSA_IO_PENDING)
        {
            if (WSAWaitForMultipleEvents(1, &SocketInfo.Overlapped.hEvent, FALSE, INFINITE, FALSE) == WAIT_TIMEOUT)
            {
                std::cout << "RecvFrom() Timeout" << std::endl;
                return FALSE;
            }
        }else
        {
            std::cout << "RecvFrom() failed with error " << WSAGetLastError() << std::endl;
            return FALSE;
        }

    }
   // std::cout << "Bytes Recieved: " << SocketInfo.BytesRECV << std::endl;
    if(!WSAGetOverlappedResult(SocketInfo.Socket, &(SocketInfo.Overlapped), &SocketInfo.BytesRECV, FALSE, &Flags))
    {
        std::cout << "ClientUDP::WSAGetOVerlappedResult failed with errno " << WSAGetLastError() << std::endl;
        return FALSE;
    }
    return TRUE;
}
bool ClientUDP::Send(char * message, int size)
{
    DWORD flags = 0;
    SocketInfo.DataBuf.buf = message;
    SocketInfo.DataBuf.len = size;
    memset(&SocketInfo.Overlapped, '\0', sizeof(WSAOVERLAPPED));
    SocketInfo.Overlapped.hEvent = WSACreateEvent();
    if (WSASendTo(SocketInfo.Socket,
        &(SocketInfo.DataBuf),
        1,
        &SocketInfo.BytesSEND,
        flags,
        (SOCKADDR *)&LocalAddr,
        sizeof(LocalAddr),
        &SocketInfo.Overlapped,
        NULL)
        < 0)
    {
        if (WSAGetLastError() == WSA_IO_PENDING)
        {
            if(WSAWaitForMultipleEvents(1, &SocketInfo.Overlapped.hEvent, FALSE, 100, FALSE) == WAIT_TIMEOUT)
            {
                std::cout << "RecvFrom() Timeout" << std::endl;
                return FALSE;
            }
           if(!WSAGetOverlappedResult(SocketInfo.Socket, &(SocketInfo.Overlapped), &SocketInfo.BytesSEND, FALSE, &Flags))
            {
                std::cout << "ClientUDP::WSAGetOVerlappedResult failed with errno " << WSAGetLastError() << std::endl;
                return FALSE;
            }
        }
    }
    return TRUE;
}

bool ClientUDP::leaveMulticast() {
    if(setsockopt(SocketInfo.Socket, IPPROTO_IP, IP_DROP_MEMBERSHIP,
                  (char *)&MulticastInfo, sizeof(MulticastInfo)) == SOCKET_ERROR)
    {
        std::cout << "Close()::setsockopt() failed with error " << WSAGetLastError() << std::endl;
        return false;
    }

    return true;
}

bool ClientUDP::Close()
{
    closesocket(SocketInfo.Socket);
    WSACleanup();
    return TRUE;
}
