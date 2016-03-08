#include "mainwindow.h"
#include <QApplication>
#include "Server/ServerUDP.h"
void StartServer();
DWORD WINAPI UDPThread(LPVOID lpParameter);

int main(int argc, char *argv[])
{
    QApplication program(argc, argv);
    MainWindow w;
    w.show();

    ServerUDP serverUDP;
    HANDLE UDPServerThread;
    DWORD UDPServerThreadID;

    if(serverUDP.InitializeSocket(DEFAULT_PORT) < 0)
        return -1;

    if(serverUDP.MulticastSettings(DEAULT_MULTICAST_IP) < 0)
        return -1;

    CreateThread(NULL, 0, UDPThread, (LPVOID)&serverUDP, 0, &UDPServerThreadID);
    return program.exec();
}

DWORD WINAPI UDPThread(LPVOID lpParameter)
{
    std::cout << "Thread created " << std::endl;
    ServerUDP * serverUDP = (ServerUDP * )lpParameter;
    while(1)
    {
        if(!serverUDP->Broadcast("Scamaz"))
            return -1;
    }
    return 0;
}



