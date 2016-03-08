#include "mainwindow.h"
#include "Client/ClientUDP.h"
#include <QApplication>
void StartServer();

int main(int argc, char *argv[])
{
    QApplication program(argc, argv);
    MainWindow w;
    w.show();

    ClientUDP clientUDP;

    if(!clientUDP.InitializeSocket(DEFAULT_PORT))
        return -1;

    if(!clientUDP.MulticastSettings(DEAULT_MULTICAST_IP))
        return -1;

    while(1)
    {
        if(!clientUDP.Recv())
            return -1;
        std::cout << "Recieved message: " << clientUDP.SocketInfo.DataBuf.buf << std::endl;
    }
    return program.exec();
}



