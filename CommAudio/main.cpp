#include "mainwindow.h"
#include "Client/ClientTCP.h"
#include <QApplication>
void loadStyleSheet();

int main(int argc, char *argv[])
{
    QApplication program(argc, argv);
    MainWindow w;
    w.show();

    loadStyleSheet();

    return program.exec();;
}


void loadStyleSheet() {
    QFile f(":qdarkstyle/style.qss");
    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
        exit(1);
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }
}
