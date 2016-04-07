#include "tcpthreadmanager.h"

DWORD WINAPI AcceptThread(LPVOID lpParam);

void TCPThreadManager::TCPReceiveThread() {
    DWORD AcceptThreadID;

    ClientTCP clientTCP;
    if(!clientTCP.InitializeSocket(DEFAULT_PORT, ipAddr.data()))
        return;

    WSAEVENT EventArray[1];
    DWORD index;
    clientTCP.WSAEvent = WSACreateEvent();
    if(clientTCP.WSAEvent == WSA_INVALID_EVENT) {
        qDebug() << "Failed to create event";
        exit(1);
    }
    EventArray[0] = clientTCP.WSAEvent;
    CreateThread(NULL, 0, AcceptThread, (LPVOID)&clientTCP, 0, &AcceptThreadID);

    while(1)
    {
        index = WSAWaitForMultipleEvents(1, EventArray, FALSE, WSA_INFINITE, TRUE);
        if(index == WSA_WAIT_FAILED) {
            qDebug() << "WSAWaitForMultipleEvents";
            exit(1);
        }
        clientTCP.Recv();
        while(clientTCP.SocketInfo.BytesRECV != 0) {
            std::cout << "Recieved message: " << clientTCP.SocketInfo.DataBuf.buf <<
                         "Size: " << clientTCP.SocketInfo.BytesRECV << std::endl;
            clientTCP.Recv();
        }
        WSAResetEvent(EventArray[index - WSA_WAIT_EVENT_0]);
    }

}


DWORD WINAPI AcceptThread(LPVOID lpParam) {
    ClientTCP * clientTCP = (ClientTCP * )lpParam;
    while(1) {
        clientTCP->acceptConnection();
        if (WSASetEvent(clientTCP->WSAEvent) == FALSE) {
                qDebug() << "EventSet failed";
                exit(1);
        }
    }
}

void TCPThreadManager::TCPThreadRequest() {
    emit TCPThreadRequested();
}
