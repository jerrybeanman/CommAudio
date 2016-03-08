#include "mainwindow.h"
#include <QApplication>
#include "Server/ServerUDP.h"
#include "Server/serverthreadcontroller.h"
void StartServer();

int main(int argc, char *argv[])
{
    QApplication program(argc, argv);
    MainWindow w;
    w.show();

    ServerThreadController test;

    return program.exec();
}



