#include "mainwindow.h"
#include "Client/ClientTCP.h"
#include "Client/completionroutine.h"
#include <QApplication>
void StartServer();
DWORD WINAPI UDPThread(LPVOID lpParameter);
DWORD WINAPI AcceptThread(LPVOID lpParam);

int main(int argc, char *argv[])
{
    QApplication program(argc, argv);
    MainWindow w;
    w.show();

    HANDLE UDPClientThread;
    DWORD UDPClientThreadID;

    ClientTCP tcp;

    if(!tcp.InitializeSocket(DEFAULT_PORT, "142.232.144.70"))
        return -1;

    CreateThread(NULL, 0, UDPThread, (LPVOID)&tcp, 0, &UDPClientThreadID);
    return program.exec();;
}

DWORD WINAPI UDPThread(LPVOID lpParameter)
{
    std::cout << "Thread created " << std::endl;
    ClientTCP * clientTCP = (ClientTCP * )lpParameter;
    HANDLE UDPClientThread;
    DWORD UDPClientThreadID;

    WSAEVENT EventArray[1];
    DWORD index;
    clientTCP->WSAEvent = WSACreateEvent();
    if(clientTCP->WSAEvent == WSA_INVALID_EVENT) {
        qDebug() << "Failed to create event";
        exit(1);
    }
    EventArray[0] = clientTCP->WSAEvent;
    CreateThread(NULL, 0, AcceptThread, (LPVOID)clientTCP, 0, &UDPClientThreadID);

    while(1)
    {
        index = WSAWaitForMultipleEvents(1, EventArray, FALSE, WSA_INFINITE, TRUE);
        if(index == WSA_WAIT_FAILED) {
            qDebug() << "WSAWaitForMultipleEvents";
            exit(1);
        }
        clientTCP->Recv();
        while(clientTCP->SocketInfo.BytesRECV != 0) {
            std::cout << "Recieved message: " << clientTCP->SocketInfo.DataBuf.buf <<
                         "Size: " << clientTCP->SocketInfo.BytesRECV << std::endl;
            clientTCP->Recv();
        }
        WSAResetEvent(EventArray[index - WSA_WAIT_EVENT_0]);
    }
    return 0;
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
