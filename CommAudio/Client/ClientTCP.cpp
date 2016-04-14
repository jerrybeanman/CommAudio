#include "ClientTCP.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	InitializeSocket
--
-- DATE:		April 11th, 2016
--
-- REVISIONS:
--
-- DESIGNER:	Ruoqi Jia
--
-- PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	bool ClientTCP::InitializeSocket(short port, char *ip)
--
-- RETURNS: true on success, false otherwise
--
-- NOTES: Initialize socket to a specific address
--------------------------------------------------------------------------------------------------------------------*/
bool ClientTCP::InitializeSocket(short port, char *ip)
{
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cout << "ClientTCP WSAStartup failed with error " << WSAGetLastError() << std::endl;
        return FALSE;
    }

    if ((SocketInfo.Socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
    {
        std::cout << "ClientTCP WSASocket() failed with error " <<  WSAGetLastError() << std::endl;
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

    if (connect (SocketInfo.Socket, (struct sockaddr *)&LocalAddr, sizeof(LocalAddr)) == -1)
        {
            qDebug() << "Can't connect to server: " << WSAGetLastError();
            return false;
        }
    return true;

}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	Recv
--
-- DATE:		April 11th, 2016
--
-- REVISIONS:
--
-- DESIGNER:	Ruoqi Jia
--
-- PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	bool ClientTCP::Recv()
--
-- RETURNS: true on success, false otherwise
--
-- NOTES: recieve from the TCP socket
--------------------------------------------------------------------------------------------------------------------*/
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
                std::cout << "Recv() Timeout" << std::endl;
                return FALSE;
            }
        } else {
            std::cout << "Recv() failed with error " << WSAGetLastError() << std::endl;
            return FALSE;
        }
    }

    if(!WSAGetOverlappedResult(SocketInfo.Socket, &(SocketInfo.Overlapped), &SocketInfo.BytesRECV, FALSE, &Flags))
    {
        std::cout << "ClientTCP::WSAGetOVerlappedResult failed with errno " << WSAGetLastError() << std::endl;
        return FALSE;
    }
    return TRUE;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	Send
--
-- DATE:		April 11th, 2016
--
-- REVISIONS:
--
-- DESIGNER:	Ruoqi Jia
--
-- PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	bool ClientTCP::Send(char * message, int size)
--
-- RETURNS: true on success, false otherwise
--
-- NOTES: Send message to the TCP socket
--------------------------------------------------------------------------------------------------------------------*/
bool ClientTCP::Send(char * message, int size)
{
    OVERLAPPED ov;
    memset(SocketInfo.Buffer, '\0', DATA_BUFSIZE);
    SocketInfo.DataBuf.buf = message;
    SocketInfo.DataBuf.len = size;

    std::cout << "ClientTCP::Send>>Length: " << size << std::endl;
    fflush(stdout);
    ZeroMemory(&ov, sizeof(OVERLAPPED));
    ov.hEvent =  WSACreateEvent();

    if (WSASend(SocketInfo.Socket,      /* Writing socket                       */
            &(SocketInfo.DataBuf),        /* Message content                      */
            1,
            NULL,                         /* Size of the message                  */
            Flags,
            &(ov),
            NULL)                       /* size of the socket address structure */
            < 0)
    {
        if (WSAGetLastError() != WSA_IO_PENDING)
        {
            std::cout << "ClientTCP::WSASend() () failed with error " << WSAGetLastError() << std::endl;
            return FALSE;
        }
        if (WSAWaitForMultipleEvents(1, &ov.hEvent, FALSE, INFINITE, FALSE) == WAIT_TIMEOUT)
        {
            std::cout << "ClientTCP::WSASend() Timeout" << std::endl;
            return FALSE;
        }
    }

    //Get the actual bytes sent.
    if(!WSAGetOverlappedResult(SocketInfo.Socket, &(ov), &SocketInfo.BytesSEND, FALSE, &Flags))
    {
        std::cout << "ClientTCP::WSAGetOverlappedResult failed with errno" << WSAGetLastError() << std::endl;
        return FALSE;
    }
    std::cerr << "ClientTCP::Broadcast>>Bytes Sent:[" << SocketInfo.BytesSEND << "]" << std::endl;
    return true;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	Close
--
-- DATE:		April 11th, 2016
--
-- REVISIONS:
--
-- DESIGNER:	Ruoqi Jia
--
-- PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	bool ClientTCP::Close()
--
-- RETURNS: true on success, false otherwise
--
-- NOTES: close socket
--------------------------------------------------------------------------------------------------------------------*/
bool ClientTCP::Close() {
    if(closesocket(SocketInfo.Socket) == SOCKET_ERROR) {
        return false;
    }
    WSACleanup();

    return true;
}

