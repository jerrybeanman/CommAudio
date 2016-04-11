#include "mainwindow.h"
#include "microphonethreadmanager.h"
#include "microphonethreadrecievingmanager.h"
#include <QFileDialog>
#include <QSound>
#include <QMediaPlayer>
#include <QBuffer>
#include <QAudioDecoder>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QSlider>
CircularBuffer cb;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    player = new QMediaPlayer(this);
    m_device = QAudioDeviceInfo::defaultOutputDevice();
    fileExists = false;
    fileLoaded = false;
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabSelected()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

QByteArray MainWindow::getServerAddress() {
    QString serverIP = ui->serverIPAddr->text();
    if(serverIP == "...") {
        return "";
    }
    return serverIP.toLocal8Bit();
}

void MainWindow::on_connectButton_pressed()
{
    serverIP = getServerAddress();
    ui->serverIPAddr->clear();
    ui->stackedWidget->setCurrentIndex(1);


    tcpThread = new QThread();

    TCPWorker = new TCPThreadManager(serverIP);
    m_generator = new DataGenerator();

    initializeUDPThread();

    TCPWorker->moveToThread(tcpThread);

    connect(TCPWorker, SIGNAL(songList(const QByteArray&)), this,
            SLOT(generatePlaylist(const QByteArray&)));
    connect(TCPWorker, SIGNAL(TCPThreadRequested()), tcpThread, SLOT(start()));
    connect(tcpThread, SIGNAL(started()), TCPWorker, SLOT(TCPReceiveThread()));
    connect(TCPWorker, SIGNAL(finished()), tcpThread, SLOT(quit()), Qt::DirectConnection);

    TCPWorker->TCPThreadRequest();
    TCPWorker->sendSongRequest(QByteArray("1"));

    //initializeMicrophoneConnection();

}
static int count;
void MainWindow::addToSongBuffer(const unsigned int size) {
    while(cb.Count != 0)
    {
        char* temp = new char[DATA_BUFSIZE];
        CBPop(&cb, temp);
        QByteArray data = QByteArray::fromRawData(temp, size);
        m_generator->AddMoreDataToBufferFromQByteArray(data, size);
        count++;
        delete temp;
    }
    if((count >= 10 )&& !(m_generator->isPlaying())) {
        play_audio();
    }
}

void MainWindow::addToSongHeader() {
    if(cb.Count != 0) {
        if(m_audioOutput != nullptr) {
            m_generator->resetPosition();
            m_audioOutput->reset();
            delete(m_audioOutput);
        }
        m_generator->RemoveBufferedData();
        count = 0;
        char* temp = new char[DATA_BUFSIZE];
        CBPop(&cb, temp);
        prepare_audio_devices(m_generator->readHeader(temp));
        delete temp;
    }
}

void MainWindow::initializeMicrophoneConnection()
{
    microphoneThread      = new QThread();
    microphoneRecvThread  = new QThread();
    MicrophoneThreadManager* microphoneWorker = new MicrophoneThreadManager();
    MicrophoneThreadRecvManager* microphoneRecvWorker = new MicrophoneThreadRecvManager();

    microphoneWorker->moveToThread(microphoneThread);
    microphoneRecvWorker->moveToThread(microphoneRecvThread);

    connect(microphoneWorker, SIGNAL(MicrophoneThreadRequested()), microphoneThread, SLOT(start()));
    connect(microphoneThread, SIGNAL(started()), microphoneWorker, SLOT(MicrohponeSendThread()));
    connect(microphoneWorker, SIGNAL(finished()), microphoneThread, SLOT(quit()), Qt::DirectConnection);

    connect(microphoneRecvWorker, SIGNAL(RecievedData(const unsigned int)), this,
            SLOT(addToSongBuffer(const unsigned int)));
    connect(microphoneRecvWorker, SIGNAL(MicrophoneRecvThreadRequested()), microphoneRecvThread, SLOT(start()));
    connect(microphoneRecvThread, SIGNAL(started()), microphoneRecvWorker, SLOT(MicrohponeRecvThread()));
    connect(microphoneRecvWorker, SIGNAL(finished()), microphoneRecvThread, SLOT(quit()), Qt::DirectConnection);

    microphoneWorker->MicrophoneThreadRequest();

}

void MainWindow::addVoiceData(const unsigned int size) {
    //TODO add voice data
}

void MainWindow::initializeUDPThread() {

    broadcastThread = new QThread();
    UDPWorker = new UDPThreadManager();

    UDPWorker->moveToThread(broadcastThread);

    connect(UDPWorker, SIGNAL(songDataReceived(const unsigned int)), this,
            SLOT(addToSongBuffer(const unsigned int)));
    connect(UDPWorker, SIGNAL(songHeader()), this,
            SLOT(addToSongHeader()));
    connect(UDPWorker, SIGNAL(UDPThreadRequested()), broadcastThread, SLOT(start()));
    connect(broadcastThread, SIGNAL(started()), UDPWorker, SLOT(UDPReceiveThread()));
    connect(UDPWorker, SIGNAL(finished()), broadcastThread, SLOT(quit()), Qt::DirectConnection);

    UDPWorker->UDPThreadRequest();
}

void MainWindow::tabSelected() {
    qDebug() << "Tab changed to: " << ui->tabWidget->currentIndex();

    if(UDPWorker != nullptr) {
        UDPWorker->closeSocket();
        broadcastThread->wait();
        delete(broadcastThread);
        delete(UDPWorker);
        UDPWorker = nullptr;
        broadcastThread = nullptr;
    }
    switch(ui->tabWidget->currentIndex()) {
        case broadcasting:
            initializeUDPThread();
            break;
        case fileTransfer:
            TCPWorker->sendSongRequest(QByteArray("1"));
            break;
        case mic:
            break;
    }
}

void MainWindow::generatePlaylist(const QByteArray& songs) {
    QListWidget *playList;
    if(ui->tabWidget->currentIndex() == broadcasting) {
        playList = ui->serverPlayList;
    } else if(ui->tabWidget->currentIndex() == fileTransfer){
        playList = ui->serverSongList;
    } else {
        return;
    }
    QList<QByteArray> songList = songs.split(' ');
    foreach (const QString& song, songList) {
        playList->addItem(song);
    }
}

void MainWindow::updateFileProgress(const int progress) {
//    if(ui->tabWidget->currentIndex() == 0) {
//        ui->songProgress->setValue(progress);
//    } else if(ui->tabWidget->currentIndex() == 1) {
//        ui->fileProgress->setValue(progress);
//    }
    Q_UNUSED(progress)
}

void MainWindow::prepare_audio_devices(QAudioFormat format)
{
    m_format = format;
    qDebug() << m_device.deviceName();

    if(!m_device.isFormatSupported(m_format))
    {
        qWarning()<<"raw audio format not supported by backend, cannot play audio.";
        return;
    }

    m_audioOutput = 0;
    m_audioOutput = new QAudioOutput(m_device, m_format, this);

}

void MainWindow::init_file()
{
    /* Purpose, split the file in half and read it in portions */
    qint64 size = m_file->size() - 44; //Size of the file minus the header.
    m_file->seek(0);
    QByteArray array = m_file->read(size/2);
    m_generator->AddMoreDataToBufferFromQByteArray(array, size/2);

    array = m_file->read(size/2);
    m_generator->AddMoreDataToBufferFromQByteArray(array, size/2);
}

void MainWindow::play_audio()
{
    if(m_generator->isPlaying())
    {
        qDebug() << "Audio file is resuming.";
        m_audioOutput->resume();
    }
    else
    {
        qDebug() << "Starting file from beginning.";

        m_generator->start();

        m_audioOutput->start(m_generator);
        m_audioOutput->setVolume(qreal(100.0f/100.0f));
    }
}

void MainWindow::on_recordButton_clicked()
{
    qDebug() << "recording starts.";
    m_recorder = new Recorder();
    m_recorder->start();
}

void MainWindow::on_playRecordingButton_clicked()
{
    m_recorder->stop();

    QByteArray array = m_recorder->readAll();
    int size = m_recorder->bytesWritten();

    prepare_audio_devices(m_recorder->fileFormat());

    m_generator = new DataGenerator();

    m_generator->AddMoreDataToBufferFromQByteArray(array, size);

    play_audio();
}

void MainWindow::on_progressBar_actionTriggered(int progress)
{
    ui->progressBar->setValue(progress);
}

void MainWindow::on_play_clicked()
{
    if(fileExists)
    {
        qDebug() << "New file.";
        delete m_file;
        delete m_generator;
        fileLoaded = false;
    }
    m_file = new WavFile(this);
    m_generator = new DataGenerator();
    connect(m_generator, SIGNAL(audioProgressChanged(int)), this, SLOT(on_progressBar_actionTriggered(int)));

    m_file->open(QFileDialog::getOpenFileName(this, tr("Select a File"), 0, tr("Music (*.wav)")));
    prepare_audio_devices(m_file->fileFormat());
    fileExists = true;
    QFileInfo fileInfo(m_file->fileName());
    QString filename(fileInfo.fileName());

    updateFileProgress(0);

    qDebug() << "Play button clicked.";
    if(!fileLoaded)
    {
        qDebug() << "Loading file contents.";
        init_file();
        qDebug() << "After init";
        fileLoaded = true;
    }

    play_audio();
}

void MainWindow::on_requestFile_clicked()
{
    QString song = ui->serverSongList->currentItem()->text();
    song.prepend("2 ");
    qDebug() << song;
    TCPWorker->sendSongRequest(song.toLocal8Bit());

}
