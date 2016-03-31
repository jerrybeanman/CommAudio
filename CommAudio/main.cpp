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

  return program.exec();
}
