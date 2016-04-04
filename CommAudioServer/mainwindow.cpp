#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QSound>
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_device = QAudioDeviceInfo::defaultOutputDevice();
    fileExists = false;
    fileLoaded = false;
    fileFinished = false;
    m_pos = 0;
    m_song_index = 0;

    load_music_files();
    ready_next_song();
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


    if(!m_file->open(QFileDialog::getOpenFileName(this, tr("Select a File"), 0, tr("Music (*.wav)"))))
    {
        m_file = 0;
        qDebug() << "Cannot open file:";
        return;
    }

    fileExists = true;
    m_generator = new DataGenerator(this);
    connect(m_generator, SIGNAL(audioProgressChanged(int)), this, SLOT(on_progressBar_actionTriggered(int)));

    //Tests


    m_file->seek(0);
    QByteArray array = m_file->read(44);
    QAudioFormat format = m_generator->readHeader(array.data());
    QAudioFormat temp = m_file->fileFormat();
    prepare_audio_devices(format);


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

bool MainWindow::prepare_audio_devices(QAudioFormat format)
{
    m_format = format;
    qDebug() << m_device.deviceName();

    if(!m_device.isFormatSupported(m_format))
    {
        qWarning()<<"raw audio format not supported by backend, cannot play audio.";
        return false;
    }

    m_audioOutput = 0;
    m_audioOutput = new QAudioOutput(m_device, m_format, this);
    qDebug() << "Properly set the media";
    return true;
}

void MainWindow::load_file()
{
    /* Purpose: split the file in half and read it in portions */
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

void MainWindow::stop_stream()
{
    qDebug() << "Stopping stream.";
    if(m_audioOutput == 0 || m_generator == 0)
    {
        qDebug() << "MainWindow::stop_stream>> m_audioOutput or m_generator was NULL";
        return;
    }

    m_audioOutput->reset();
    m_generator->resetPosition();
    *song_stream_data = 0;
    song_size = 0;
    streaming = false;
}

void MainWindow::load_music_files()
{
    int old_size = m_music_files.size();

    m_dir = QDir(MUSIC_DIRECTORY);

    // No dots, no hidden folders/file, show files only. After those filters, sort it by name.
    m_music_files = m_dir.entryList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::Files, QDir::Name);//(QDir::Filter::Files,QDir::SortFlag::NoSort)

    m_music_files.sort();

    if(old_size != m_music_files.size()) // Number of songs changed? Redo the entire list.
    {
        populate_songlist();
    }
}

void MainWindow::move_song_index(bool previous)
{
    if(!previous)
    {
        m_song_index++;
        if(m_song_index > m_music_files.size() - 1)
        {
            m_song_index = 0;
        }
    }
    else
    {
        m_song_index--;
        if(m_song_index < 0)
        {
            m_song_index = m_music_files.size() - 1;
        }
    }

}

bool MainWindow::ready_next_song(bool previous)
{
    /*
    bool prevState = streaming; // Temporary keep track of the previous state.
    streaming = !streaming; //switching the streaming state

    qDebug() << "FileExists:" << fileExists;


    if(!streaming) // Start stream
    {
        connect(m_generator, SIGNAL(dataAvailable(int)), this, SLOT(handleDataAvailable(int)));
        connect(m_generator, SIGNAL(dataFinished()), this, SLOT(handleDataFinished()));

        qDebug() << "Stream button clicked.";
        if(!fileLoaded)
        {
            qDebug() << "Loading file contents.";
            load_file();
            fileLoaded = true;
        }
        m_stream_size = 44;
        song_size = &m_stream_size;
        *song_stream_data = m_generator->getExternalReference()->data();
        play_audio();

    }
    */
    if(fileExists)
    {
        delete_old_song();
    }
    m_file = new WavFile(this);

    // Handle moving to the next song when the song is finished
    QString song_filename = MUSIC_DIRECTORY + m_music_files[m_song_index];
    if(!m_file->open(song_filename))
    {
        m_file = 0;
        qDebug() << "Cannot open file:";
        return false;
    }

    fileExists = true;
    m_generator = new DataGenerator(this);
    connect(m_generator, SIGNAL(audioProgressChanged(int)), this, SLOT(on_progressBar_actionTriggered(int)));

    //Prepare the audio device to stream
    m_file->seek(0);
    QByteArray array = m_file->read(44);
    QAudioFormat format = m_generator->readHeader(array.data());
    if(!prepare_audio_devices(format))
    {
        qWarning() << "MainWindow::ready_next_song>>Failed preparing the next song";
        return false;
    }

    if(!fileLoaded)
    {
        qDebug() << "Loading file contents.";
        load_file();
        fileLoaded = true;
    }

    song_selected_update(previous);

    if(!prepare_stream())
    {
        qWarning() << "Prepare stream failed.";
        // Handle stream error here.
    }

    play_audio();

    return true;
}


void MainWindow::song_selected_update(bool previous)
{
    if (ui->listWidget_2->count() > 0) {

        // Change the previously selected song's colour back to normal.
        int old_song_index;

        if(previous)
        {
            old_song_index = m_song_index + 1;
            if(old_song_index == ui->listWidget_2->count())
                old_song_index = 0;
        }
        else
        {
            old_song_index = m_song_index - 1;
            if(old_song_index == -1)
                old_song_index = ui->listWidget_2->count() - 1;
        }

        QListWidgetItem* olditem = ui->listWidget_2->item(old_song_index);
        olditem->setForeground(Qt::black);
        olditem->setBackground(Qt::white);

        // Change the current song's colour to white on blue
        QListWidgetItem* newitem = ui->listWidget_2->item(m_song_index);
        newitem->setForeground(Qt::white);
        newitem->setBackground(Qt::blue);

        ui->listWidget_2->setFocus();
    }
}

bool MainWindow::delete_old_song()
{   
    if(m_file != 0) // File must exist
    {
        while(*song_size != 0) // Allow the remaining piece of the song to send.
        {
            ;
        }

        qDebug() << "Disposing of old song.";
        delete m_file;
        fileLoaded = false;

        m_generator->RemoveBufferedData();

        return true;
    }
    qDebug() << "No existing song, no deletion.";
    return false;
}

void MainWindow::populate_songlist()
{
    while(ui->listWidget_2->count() > 0)
    {
        QListWidgetItem* temp = ui->listWidget_2->takeItem(0);
        if(temp != 0)
            delete temp;
    }
    ui->listWidget_2->addItems(m_music_files);
}

std::string MainWindow::get_all_songs()
{
    return m_music_files.join('~').toStdString();
}

void MainWindow::split_songs_from_string(std::string combinedString)
{
    QString combo;
    combo.fromStdString(combinedString);

    QStringList server_music_files = combo.split(",", QString::SkipEmptyParts);
}

bool MainWindow::prepare_stream()
{
    if(!streaming) // Start stream
    {
        streaming = true;

        connect(m_generator, SIGNAL(dataAvailable(int)), this, SLOT(handleDataAvailable(int)));
        connect(m_generator, SIGNAL(dataFinished()), this, SLOT(handleDataFinished()));

        qDebug() << "MainWindow::prepare_stream>>Sending Header.";

        *song_stream_data = m_generator->getExternalReference()->data();
        m_stream_size = 44;
        song_size = &m_stream_size;

        return true;
    }

    qDebug() << "MainWindow::prepare_stream>>Header was not sent, streaming set to true when it should be false.";
    return false;
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

void MainWindow::on_pauseButton_clicked() // Pause the music
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
    if(progress == 100)
    {
        move_song_index();
        ready_next_song();
    }
}

void MainWindow::on_streamButton_clicked(bool checked)
{
    Q_UNUSED(checked);
    bool prevState = streaming; // Temporary keep track of the previous state.
    streaming = !streaming; //switching the streaming state

    qDebug() << "FileExists:" << fileExists;

    if(!fileExists)
    {
        qDebug() << "Load a file before hitting the stream button.";
        return;
    }
    if(m_generator == 0)
    {
        qDebug() << "No generator.";
        return;
    }

    if(!streaming) // Start stream
    {
        connect(m_generator, SIGNAL(dataAvailable(int)), this, SLOT(handleDataAvailable(int)));
        connect(m_generator, SIGNAL(dataFinished()), this, SLOT(handleDataFinished()));

        qDebug() << "Stream button clicked.";
        if(!fileLoaded)
        {
            qDebug() << "Loading file contents.";
            load_file();
            fileLoaded = true;
        }
        m_stream_size = 44;
        song_size = &m_stream_size;
        *song_stream_data = m_generator->getExternalReference()->data();
        play_audio();

    }
    else if(streaming && m_generator->isPlaying())
    {
        qDebug() << "Stopping stream";
        stop_stream();
    }
    else // Streaming button clicked but it was unable to proceed
    {
        streaming = prevState;
    }
}

void MainWindow::handleDataAvailable(int len)
{
    *song_size += static_cast <DWORD>(len);
    /*
    if(fileFinished)
        m_pos = 0;
    */
}

void MainWindow::handleDataFinished()
{
    qDebug() << "Data has finished sending.";
    fileFinished = true;
    //streaming = false;
    move_song_index();
}

void MainWindow::on_pauseBtn_clicked()
{
    if(!fileExists)
        return;

    if(m_generator->isPlaying())
    {
        qDebug() << "Pause button clicked.";
        m_audioOutput->suspend();
    }
}

void MainWindow::on_nextsongBtn_clicked()
{
    if(m_generator->isPlaying())
    {
        qDebug() << "Pause button clicked.";
        m_audioOutput->suspend();
    }

    streaming = false;
    move_song_index();
    ready_next_song();
}

void MainWindow::on_prevsongBtn_clicked()
{
    if(m_generator->isPlaying())
    {
        qDebug() << "Pause button clicked.";
        m_audioOutput->suspend();
    }

    streaming = false;
    move_song_index(true);
    ready_next_song(true);
}
