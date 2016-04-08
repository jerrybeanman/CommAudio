#include "windowadapter.h"
#include <iostream>
#include <QFileDialog>
#include <QSound>
#include <QMediaPlayer>
#include <QBuffer>
#include <QAudioDecoder>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QSlider>
#include <QMetaObject>
CircularBuffer cb;


WindowAdapter::WindowAdapter()
{

    //ui->setupUi(this);
    m_device = QAudioDeviceInfo::defaultOutputDevice();
    fileExists = false;
    fileLoaded = false;
    //connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabSelected()));
    data_file = new QFile("output.wav");
    if(!data_file->open(QIODevice::WriteOnly)) {
        std::cerr << data_file->errorString().toStdString();
        return;
    }


}

QByteArray WindowAdapter::getServerAddress(QString IPAdd) {
    //QString serverIP = ui->serverIPAddr->text();
    QString serverIP = IPAdd;
    if(serverIP == "...") {
        return "";
    }
    return serverIP.toLocal8Bit();
}

void WindowAdapter::on_connectButton_pressed(QString IPAdd)
{

    std::cerr << IPAdd.toStdString();

    serverIP = getServerAddress(IPAdd);
    //ui->serverIPAddr->clear();
    //ui->stackedWidget->setCurrentIndex(1);


    tcpThread = new QThread();

    QByteArray testIP(serverIP);
    TCPThreadManager* TCPWorker = new TCPThreadManager(testIP);
    m_generator = new DataGenerator();

    initializeUDPThread();

    TCPWorker->moveToThread(tcpThread);

    connect(TCPWorker, SIGNAL(TCPThreadRequested()), tcpThread, SLOT(start()));
    connect(tcpThread, SIGNAL(started()), TCPWorker, SLOT(TCPReceiveThread()));
    connect(TCPWorker, SIGNAL(finished()), tcpThread, SLOT(quit()), Qt::DirectConnection);

    TCPWorker->TCPThreadRequest();
    ClientTCP startConnection;
    startConnection.connectToServer(TCP_PORT, serverIP.data());
    startConnection.Send("1", 2);

}
static int count;
void WindowAdapter::addToSongBuffer(const unsigned int size)
{
    while(cb.Count != 0)
    {
        std::cerr << "Before the pop";
        char* temp = new char[DATA_BUFSIZE];
        CBPop(&cb, temp);
        QByteArray data = QByteArray::fromRawData(temp, size);
        std::cerr << "Before the generator";
        m_generator->AddMoreDataToBufferFromQByteArray(data, size);
        std::cerr << "After the generator";
        count++;
        delete temp;
        std::cerr << "After the pop";
    }
    if((count >= 10 )&& !(m_generator->isPlaying())) {
        std::cerr << "Playing";
        play_audio();
        std::cerr << "Playing donerino";
    }
}

void WindowAdapter::addToSongHeader(const unsigned int size) {
    if(cb.Count != 0) {
        if(m_audioOutput != nullptr) {
            std::cerr << "Deleting the m_audioOutput";
            m_generator->resetPosition();
            m_audioOutput->reset();
            delete(m_audioOutput);
            std::cerr << "Finished deleting";
        }
       std::cerr << "Removing";
        m_generator->RemoveBufferedData();
        count = 0;
        char* temp = new char[DATA_BUFSIZE];
        CBPop(&cb, temp);
        prepare_audio_devices(m_generator->readHeader(temp));
        delete temp;
    }
}

void WindowAdapter::initializeUDPThread() {

    broadcastThread = new QThread();
    UDPWorker = new UDPThreadManager();

    UDPWorker->moveToThread(broadcastThread);

    connect(UDPWorker, SIGNAL(songDataReceived(const unsigned int)), this,
            SLOT(addToSongBuffer(const unsigned int)));
    connect(UDPWorker, SIGNAL(songHeader(const unsigned int)), this,
            SLOT(addToSongHeader(const unsigned int)));
    connect(UDPWorker, SIGNAL(UDPThreadRequested()), broadcastThread, SLOT(start()));
    connect(broadcastThread, SIGNAL(started()), UDPWorker, SLOT(UDPReceiveThread()));
    connect(UDPWorker, SIGNAL(finished()), broadcastThread, SLOT(quit()), Qt::DirectConnection);

    UDPWorker->UDPThreadRequest();
}

void WindowAdapter::tabSelected() {
    //std::cerr << "Tab changed to: " << ui->tabWidget->currentIndex();

    if(UDPWorker != nullptr) {
        UDPWorker->closeSocket();
        broadcastThread->wait();
        delete(broadcastThread);
        delete(UDPWorker);
        UDPWorker = nullptr;
        broadcastThread = nullptr;
    }
//    switch(ui->tabWidget->currentIndex()) {
//        case broadcasting:
//            generatePlaylist("Song1 Song2 Song3 Song4 Song5 Song6");
//            initializeUDPThread();
//            break;
//        case fileTransfer:
//            break;
//        case mic:
//            break;
//    }
}

void WindowAdapter::generatePlaylist(QByteArray songs) {
    QList<QByteArray> songList = songs.split(' ');
//    foreach (const QString& song, songList) {
//        ui->serverSongList->addItem(song);
//    }
}

void WindowAdapter::on_filePicker_pressed() {

    if(fileExists)
    {
        std::cerr << "New file.";
        delete m_file;
        delete m_generator;
        fileLoaded = false;
    }
    m_file = new WavFile(this);
    m_generator = new DataGenerator();
    connect(m_generator, SIGNAL(audioProgressChanged(int)), this, SLOT(on_progressBar_actionTriggered(int)));

    m_file->open(QFileDialog::getOpenFileName(nullptr, tr("Select a File"), 0, tr("Music (*.wav)")));
    prepare_audio_devices(m_file->fileFormat());
    fileExists = true;
    QFileInfo fileInfo(m_file->fileName());
    QString filename(fileInfo.fileName());

   // ui->listWidget_2->addItem(filename);

    updateFileProgress(0);
}

void WindowAdapter::updateFileProgress(const int progress) {
//    if(ui->tabWidget->currentIndex() == 0) {
//        ui->songProgress->setValue(progress);
//    } else if(ui->tabWidget->currentIndex() == 1) {
//        ui->fileProgress->setValue(progress);
//    }
    std::cerr << "Is this even hit?";
    Q_UNUSED(progress)
}

void WindowAdapter::prepare_audio_devices(QAudioFormat format)
{
    std::cerr << "in prepare_audio_device";
    m_format = format;
    m_device = QAudioDeviceInfo::defaultOutputDevice();

    std::cerr << m_device.deviceName().toStdString();

    if(!m_device.isFormatSupported(m_format))
    {
        qWarning()<<"raw audio format not supported by backend, cannot play audio.";
        return;
    }

    std::cerr << "Poop";
    m_audioOutput = 0;
    std::cerr << "It's already null";
    m_audioOutput = new QAudioOutput(m_device, m_format);
    std::cerr << "Successerino";
}

void WindowAdapter::init_file()
{
    /* Purpose, split the file in half and read it in portions */
    qint64 size = m_file->size() - 44; //Size of the file minus the header.
    m_file->seek(0);
    QByteArray array = m_file->read(size/2);
    m_generator->AddMoreDataToBufferFromQByteArray(array, size/2);

    array = m_file->read(size/2);
    m_generator->AddMoreDataToBufferFromQByteArray(array, size/2);
}

void WindowAdapter::play_audio()
{
    if(m_generator->isPlaying())
    {
        std::cerr << "Audio file is resuming.";
        m_audioOutput->resume();
    }
    else
    {
        std::cerr << "Starting file from beginning.";

        m_generator->start();

        m_audioOutput->start(m_generator);
        m_audioOutput->setVolume(qreal(100.0f/100.0f));
    }
}

void WindowAdapter::on_pushButton_clicked()
{
    std::cerr << "Play button clicked.";
    if(!fileLoaded)
    {
        std::cerr << "Loading file contents.";
        init_file();
        std::cerr << "After init";
        fileLoaded = true;
    }

    play_audio();
}

void WindowAdapter::on_recordButton_clicked()
{
    std::cerr << "recording starts.";
    m_recorder = new Recorder();
    m_recorder->start();
}

void WindowAdapter::on_playRecordingButton_clicked()
{
    m_recorder->stop();

    QByteArray array = m_recorder->readAll();
    int size = m_recorder->bytesWritten();

    prepare_audio_devices(m_recorder->fileFormat());

    m_generator = new DataGenerator();

    m_generator->AddMoreDataToBufferFromQByteArray(array, size);

    play_audio();
}

void WindowAdapter::on_progressBar_actionTriggered(int progress)
{
   // ui->progressBar->setValue(progress);
}

void WindowAdapter::printHello()
{
    std::cout << "Hello world!" << std::endl;
}
