#include "mainwindow.h"
#include <QApplication>
#include "soundmanager.h"
#include "filemanager.h"

DWORD WINAPI UDPThread(LPVOID lpParameter);

int main(int argc, char *argv[])
{
    QApplication program(argc, argv);
    MainWindow w;
    w.show();

    ServerUDP serverUDP;

    if(!StartSoundManager())
    {
    	std::cout << "StartSoundManager() failed to start" << std::endl;
    	return -1;
    }
   /* if(!StartFileManager())
    {
    	std::cout << "StartFileManager() failed to start" << std::endl;
    }*/

    return program.exec();
}





