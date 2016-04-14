#include "mainwindow.h"
#include <QFileDialog>
#include <QSound>
#include <QMediaPlayer>
#include <QBuffer>
#include <QAudioDecoder>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QSlider>
#include <QInputDialog>
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
    volume = 100;

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
    ui->volumeSlider->setSliderPosition(volume);

    tcpThread = new QThread();

    TCPWorker = new TCPThreadManager(serverIP);
    m_generator = new DataGenerator();

    initializeUDPThread();

    TCPWorker->moveToThread(tcpThread);

    connect(TCPWorker, SIGNAL(songHeader()), this,
            SLOT(addSongHeader()));
    connect(TCPWorker, SIGNAL(songNameReceived(const QByteArray)), this,
            SLOT(setSong(const QByteArray)));
    connect(TCPWorker, SIGNAL(songList(const QByteArray&)), this,
            SLOT(generatePlaylist(const QByteArray&)));
    connect(TCPWorker, SIGNAL(TCPThreadRequested()), tcpThread, SLOT(start()));
    connect(tcpThread, SIGNAL(started()), TCPWorker, SLOT(TCPReceiveThread()));
    connect(TCPWorker, SIGNAL(finished()), tcpThread, SLOT(quit()), Qt::DirectConnection);

    TCPWorker->TCPThreadRequest();
    TCPWorker->sendSongRequest(QByteArray("1"));

}
static int count;
void MainWindow::addToSongBuffer(const unsigned int size) {
    if(!headerReceived) {
        return;
    }
    if((count >= 0) && !(m_generator->isPlaying())) {
        m_generator->RestartPlaying();
        play_audio();
    }
    while(cb.Count != 0)
    {
        char* temp = new char[DATA_BUFSIZE];
        CBPop(&cb, temp);
        QByteArray data = QByteArray::fromRawData(temp, size);
        m_generator->AddMoreDataToBufferFromQByteArray(data, size);
        count++;
        delete temp;
    }
}


void MainWindow::addSongHeader() {
    if(cbControl.Count != 0) {
        if(m_audioOutput != nullptr) {
            m_generator->resetPosition();
            m_generator->RemoveBufferedData();
            m_audioOutput->reset();
            delete(m_audioOutput);
        }
        headerReceived = true;

        if(m_generator == 0)
        {
            m_generator = new DataGenerator();
        }
        m_generator->RemoveBufferedData();
        count = 0;
        char* temp = new char[DATA_BUFSIZE];
        CBPop(&cbControl, temp);
        prepare_audio_devices(m_generator->readHeader(temp));
        delete temp;
    }
}

void MainWindow::setSong(const QByteArray name) {
    ui->songName->setText(QString(name).prepend("Song Name: "));
}

void MainWindow::initializeMicrophoneConnection()
{
    microphoneThread      = new QThread();
    microphoneRecvThread  = new QThread();
    microphoneWorker = new MicrophoneThreadManager(peerIP);
    microphoneRecvWorker = new MicrophoneThreadRecvManager(peerIP);

    microphoneWorker->moveToThread(microphoneThread);
    microphoneRecvWorker->moveToThread(microphoneRecvThread);

    connect(microphoneWorker, SIGNAL(MicrophoneThreadRequested()), microphoneThread, SLOT(start()));
    connect(microphoneThread, SIGNAL(started()), microphoneWorker, SLOT(MicrohponeSendThread()));
    connect(microphoneWorker, SIGNAL(finished()), microphoneThread, SLOT(quit()), Qt::DirectConnection);

    microphoneWorker->MicrophoneThreadRequest();

    connect(microphoneRecvWorker, SIGNAL(RecievedData(const unsigned int)), this,
            SLOT(handleVoiceDataAvailable(const unsigned int)));
    connect(microphoneRecvWorker, SIGNAL(MicrophoneRecvThreadRequested()), microphoneRecvThread, SLOT(start()));
    connect(microphoneRecvThread, SIGNAL(started()), microphoneRecvWorker, SLOT(MicrohponeRecvThread()));
    connect(microphoneRecvWorker, SIGNAL(finished()), microphoneRecvThread, SLOT(quit()), Qt::DirectConnection);

    microphoneRecvWorker->MicrophoneRecvThreadRequest();

}

void MainWindow::initializeUDPThread() {

    broadcastThread = new QThread();
    UDPWorker = new UDPThreadManager();

    UDPWorker->moveToThread(broadcastThread);

    connect(UDPWorker, SIGNAL(songDataReceived(const unsigned int)), this,
            SLOT(addToSongBuffer(const unsigned int)));
    connect(UDPWorker, SIGNAL(UDPThreadRequested()), broadcastThread, SLOT(start()));
    connect(broadcastThread, SIGNAL(started()), UDPWorker, SLOT(UDPReceiveThread()));
    connect(UDPWorker, SIGNAL(finished()), broadcastThread, SLOT(quit()), Qt::DirectConnection);

    UDPWorker->UDPThreadRequest();
}

void MainWindow::tabSelected() {
    qDebug() << "Tab changed to: " << ui->tabWidget->currentIndex();
    ui->serverPlayList->clear();
    ui->serverSongList->clear();
    ui->peerIP->clear();
    ui->songName->clear();
    if(UDPWorker != nullptr) {
        UDPWorker->closeSocket();
        broadcastThread->wait();
        delete(broadcastThread);
        delete(UDPWorker);
        UDPWorker = nullptr;
    }

    if(microphoneWorker != nullptr) {
        microphoneWorker->closeSocket();
        microphoneRecvWorker->closeSocket();
        microphoneRecvThread->wait();
        microphoneThread->wait();
        delete(microphoneRecvThread);
        delete(microphoneThread);
        microphoneWorker = nullptr;

    }

    if(m_audioOutput != 0) {
        m_generator->resetPosition();
        m_audioOutput->reset();
        m_generator->RemoveBufferedData();
        m_generator = 0;
        m_audioOutput = 0;
    }

    if(recording) {
        m_recorder->stop();
        delete(m_recorder);
        delete(m_voice_generator);
        m_recorder = nullptr;
        m_voice_generator = nullptr;
        recording = false;
    }

    switch(ui->tabWidget->currentIndex()) {
        case broadcasting:
            initializeUDPThread();
            TCPWorker->sendSongRequest(QByteArray("1"));
            break;
        case fileTransfer:
            TCPWorker->sendSongRequest(QByteArray("3"));
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
    QList<QByteArray> songList = songs.split('@');
    foreach (const QString& song, songList) {
        playList->addItem(song);
    }
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

    std::cerr << "Changed m_audioOuput (prepare)\n";
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

        m_audioOutput->setVolume(qreal(volume/100.0f));
    }
}

void MainWindow::play_voice()
{
    if(m_recorder == 0)
        m_recorder = new Recorder();

    if(!ReceivingVoice)
    {
        qDebug() << "Hit";
        ReceivingVoice = true;

        std::cerr << "play_voice::new recorder\n";
        m_recorder = new Recorder();
        prepare_audio_devices(m_recorder->fileFormat());

        m_voice_generator->start();
        m_audioOutput->start(m_voice_generator);
        m_audioOutput->setVolume(qreal(volume/100.0f));
    }
    else
    {
        if(m_voice_generator->isPlaying())
        {
            m_audioOutput->resume();
        }

        //Data is being received here, handle lag here.
    }
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
    TCPWorker->sendSongRequest(song.toLocal8Bit());

}

void MainWindow::handleVoiceDataAvailable(const unsigned int len)
{
    if(m_voice_generator == nullptr)
    {
        m_voice_generator = new RecordGenerator();
    }
    std::cerr << "handleVoiceData>>count:" << cbMic.Count << std::endl;
    char* buf = (char*)malloc(DATA_BUFSIZE);
    if(cbMic.Count != 0)
    {

        CBPop(&cbMic, buf);
        //emit to TCP that this is available

        QByteArray data = QByteArray::fromRawData(buf, len);

        qDebug() << "Voice size: " << len;
        m_voice_generator->AddMoreDataToBufferFromQByteArray(data, data.size());
        play_voice();
    }
    free(buf);
}

void MainWindow::on_peerConnect_clicked()
{
    peerIP = ui->peerIP->text().toLocal8Bit();

    initializeMicrophoneConnection();

    qDebug() << "recording starts.";
    m_recorder = new Recorder();

    m_recorder->start();

    recording = true;
}

void MainWindow::on_volumeSlider_sliderMoved(int position)
{
    volume = position;
    //qDebug() << "Howdy";

    if(m_audioOutput != nullptr)
    {
        m_audioOutput->setVolume(qreal(((float)position)/100.0f));
    }
}
