#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "globals.h"
#include "Server/ServerUDP.h"


void StartServer()
{
    ServerUDP serverUDP;
    if(serverUDP.InitializeSocket(DEFAULT_PORT) < 0)
        return;
    std::cout << "Socket Created" << std::endl;


    if(serverUDP.MulticastSettings(DEAULT_MULTICAST_IP) < 0)
        return;

    serverUDP.start();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    StartServer();
}
