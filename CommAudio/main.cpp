#include "mainwindow.h"
#include <QApplication>
#include "Server/ServerUDP.h"

int main(int argc, char *argv[])
{
    QApplication program(argc, argv);
    MainWindow w;
    w.show();

    //QObject::connect(&serverUDP, SIGNAL(finished()), &program, SLOT(quit()));


    return program.exec();
}

