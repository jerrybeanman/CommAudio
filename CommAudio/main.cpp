#include "mainwindow.h"
#include "Client/ClientUDP.h"
#include <QApplication>
void StartServer();
DWORD WINAPI UDPThread(LPVOID lpParameter);

int main(int argc, char *argv[])
{
    QApplication program(argc, argv);
    MainWindow w;
    w.show();

    ClientUDP clientUDP;
    HANDLE UDPClientThread;
    DWORD UDPClientThreadID;

    if(!clientUDP.InitializeSocket(DEFAULT_PORT))
        return -1;

    if(!clientUDP.MulticastSettings(DEAULT_MULTICAST_IP))
        return -1;

    CreateThread(NULL, 0, UDPThread, (LPVOID)&clientUDP, 0, &UDPClientThreadID);
    return program.exec();
}

DWORD WINAPI UDPThread(LPVOID lpParameter)
{
    std::cout << "Thread created " << std::endl;
    ClientUDP * clientUDP = (ClientUDP * )lpParameter;
    while(1)
    {
        DWORD BytesSent;
        if(!clientUDP->Recv())
            return -1;
        std::cout << "Recieved message: " << clientUDP->SocketInfo.DataBuf.buf << std::endl;
    }
    return 0;
}

