#include "mainwindow.h"
#include <QApplication>
#include "soundmanager.h"
#include "filemanager.h"
#include <direct.h>
#include <sstream>
#define GetCurrentDir _getcwd

std::vector<std::string> FileNames;
char** song_stream_data = (char**) malloc(sizeof(char*));
DWORD* song_size = 0;

int main(int argc, char *argv[])
{
    QApplication program(argc, argv);
    MainWindow* w = new MainWindow();
    w->show();
    char current_dir[200];
    if (!GetCurrentDir(current_dir, sizeof(current_dir)))
    {
        return errno;
    }
    std::stringstream ss(w->get_all_songs());
    std::string buf;
    while(ss >> buf)
    {
        FileNames.push_back((buf));
    }

    /*if(!StartSoundManager())
    {
        std::cout << "StartSoundManager() failed to start" << std::endl;
        return -1;
    }*/
   if(!StartFileManager())
    {
        std::cout << "StartFileManager() failed to start" << std::endl;
    }

    return program.exec();
}
