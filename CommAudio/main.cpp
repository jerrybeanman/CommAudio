#include "mainwindow.h"
#include <QApplication>
void StartServer();

int main(int argc, char *argv[])
{
    QApplication program(argc, argv);
    MainWindow w;
    w.show();

    return program.exec();
}



