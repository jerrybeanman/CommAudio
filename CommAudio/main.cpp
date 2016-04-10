#include "mainwindow.h"
#include "Client/ClientTCP.h"
#include <QApplication>
void StartServer();
DWORD WINAPI UDPThread(LPVOID lpParameter);
DWORD WINAPI AcceptThread(LPVOID lpParam);

int main(int argc, char *argv[])
{
    QApplication program(argc, argv);
    MainWindow w;
    w.show();

    return program.exec();
}
