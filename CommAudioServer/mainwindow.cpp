#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QSound>
#include <QMediaPlayer>
#include <QBuffer>
#include <QAudioDecoder>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QSlider>
#include "datagenerator.h"
#include "wavfile.h"
#include "globals.h"
#include "soundmanager.h"

MainWindow::MainWindow(char** stream, QWidget *parent) :
    QMainWindow(parent),
    m_stream_data(stream),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    player = new QMediaPlayer(this);
    m_device = QAudioDeviceInfo::defaultOutputDevice();
    fileExists = false;
    fileLoaded = false;
    fileFinished = false;
    m_pos = 0;
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
    generatePlaylist("Song1 Song2 Song3 Song4 Song5 Song6");
}

void MainWindow::generatePlaylist(QByteArray songs) {
    QList<QByteArray> songList = songs.split(' ');
    foreach (const QString& song, songList) {
        ui->serverSongList->addItem(song);
    }
}

void MainWindow::on_filePicker_pressed() {

    if(fileExists)
    {
        qDebug() << "New file.";
        delete m_file;
        delete m_generator;
        fileLoaded = false;
    }
    m_file = new WavFile(this);
    m_generator = new DataGenerator(this);
    connect(m_generator, SIGNAL(audioProgressChanged(int)), this, SLOT(on_progressBar_actionTriggered(int)));

    m_file->open(QFileDialog::getOpenFileName(this, tr("Select a File"), 0, tr("Music (*.wav)")));
    prepare_audio_devices(m_file->fileFormat());
    fileExists = true;

    QFileInfo fileInfo(m_file->fileName());
    QString filename(fileInfo.fileName());
    ui->listWidget_2->addItem(filename);

    updateFileProgress(0);
}

void MainWindow::updateFileProgress(const int progress) {
//    if(ui->tabWidget->currentIndex() == 0) {
//        ui->songProgress->setValue(progress);
//    } else if(ui->tabWidget->currentIndex() == 1) {
//        ui->fileProgress->setValue(progress);
//    }
    Q_UNUSED(progress);
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

void MainWindow::load_file()
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

/*
void MainWindow::on_stopButton_clicked() // Stop the music
{
    if(!fileExists)
        return;

    if(m_generator->isPlaying())
    {
        qDebug() << "Stop button clicked.";
        m_audioOutput->reset();
        fileLoaded = false;
        m_generator->resetPosition();
    }
}

void MainWindow::on_pauseButton_clicked()
{
    if(!fileExists)
        return;

    if(m_generator->isPlaying())
    {
        qDebug() << "Pause button clicked.";
        m_audioOutput->suspend();
    }
}


*/

void MainWindow::on_pushButton_clicked()
{
    if(!fileExists)
        return;

    qDebug() << "Play button clicked.";
    if(!fileLoaded)
    {
        qDebug() << "Loading file contents.";
        load_file();
        qDebug() << "After init";
        fileLoaded = true;
    }

    play_audio();
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

    m_generator = new DataGenerator(this);

    m_generator->AddMoreDataToBufferFromQByteArray(array, size);

    play_audio();
}

void MainWindow::on_progressBar_actionTriggered(int progress)
{
    ui->progressBar->setValue(progress);
}

void MainWindow::on_streamButton_clicked(bool checked)
{
    qDebug() << "stream button clicked:" << checked;
    if(!checked && m_generator != 0) // Start stream
    {
        connect(m_generator, SIGNAL(dataAvailable(int)), this, SLOT(handleDataAvailable(int)));
        connect(m_generator, SIGNAL(dataFinished()), this, SLOT(handleDataFinished()));

        m_stream_size = 0;
        song_size = &m_stream_size;
        qDebug() << "Play button clicked.";
        if(!fileLoaded)
        {
            qDebug() << "Loading file contents.";
            load_file();
            fileLoaded = true;
        }
        play_audio();
        *song_stream_data = m_generator->getExternalReference()->data();
    }
    else        //Stop stream
    {
        streaming = false;
        // stop streaming somehow here.
    }
}

void MainWindow::handleDataAvailable(int len)
{
    *song_size += static_cast <DWORD>(len);
    if(fileFinished)
        m_pos = 0;

}

void MainWindow::handleDataFinished()
{
    qDebug() << "Data has finished sending.";
    fileFinished = true;
    /*
     *
     * Do stuff here
     */
}
