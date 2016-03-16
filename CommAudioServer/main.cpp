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
    std::cout << "before get file" << std::endl;
    std::vector<std::string> s = get_all_files_names_within_folder(L"C:\\Users\\Jerry\\Dropbox\\BCIT_CST\\Term_4\\4985\\CommAudio\\CommAudio\\*.*");
    for(auto const  &x : s)
    {
        std::cout << x << std::endl;
    }
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





