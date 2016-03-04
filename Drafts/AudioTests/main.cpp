#include "AudioStream.h"
#include "generator.h"
#include "player.h"
#include "playerscontrols.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow audio;
    audio.show();

    //Player player;
    //Generator generator;
    //player.show();

    return app.exec();
}
