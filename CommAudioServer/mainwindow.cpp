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
#include "Audio/datagenerator.h"
#include "Audio/wavfile.h"
#include "globals.h"
#include "soundmanager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_device = QAudioDeviceInfo::defaultOutputDevice();
    m_song_generator = new DataGenerator(this);
    fileExists = false;
    fileLoaded = false;
    fileFinished = false;
    recording = false;
    m_pos = 0;
    m_song_index = 0;



    load_music_files();
    //ready_next_song();
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
        delete m_song_generator;
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
    m_song_generator = new DataGenerator(this);
    connect(m_song_generator, SIGNAL(audioProgressChanged(int)), this, SLOT(on_progressBar_actionTriggered(int)));

    //Tests


    m_file->seek(0);
    QByteArray array = m_file->read(44);
    QAudioFormat format = m_song_generator->readHeader(array.data());
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

    //delete m_audioOutput;
    m_audioOutput = new QAudioOutput(m_device, m_format, this);
    qDebug() << "Properly set the media";
    return true;
}

void MainWindow::load_file()
{
    if(fileExists)
    {
        m_file->seek(0);
        QByteArray array = m_file->readAll();
        m_song_generator->AddMoreDataToBufferFromQByteArray(array, array.size());
    }
}

void MainWindow::play_audio()
{
    if(m_song_generator->isPlaying())
    {
        qDebug() << "Audio file is resuming.";
        m_audioOutput->resume();
    }
    else
    {
        //qDebug() << "Starting file from beginning.";

        m_song_generator->start();

        m_audioOutput->start(m_song_generator);
        m_audioOutput->setVolume(qreal(100.0f/100.0f));
    }
}

void MainWindow::play_voice()
{
    if(m_voice_generator->isPlaying())
    {
        //We are assuming that the voice will always be playing.
        /*//qDebug() << "Audio file is resuming.";
        m_audioOutput->reset();
        m_audioOutput->start(m_voice_generator);
        m_audioOutput->setVolume(qreal(100.0f/100.0f));*/
    }
    else
    {
        //qDebug() << "Starting file from beginning.";
        m_voice_generator->start();

        m_audioOutput->start(m_voice_generator);
        m_audioOutput->setVolume(qreal(100.0f/100.0f));
    }
}

void MainWindow::stop_stream()
{
    qDebug() << "Stopping stream.";
    if(m_audioOutput == 0 || m_song_generator == 0)
    {
        qDebug() << "MainWindow::stop_stream>> m_audioOutput or m_song_generator was NULL";
        return;
    }

    m_audioOutput->reset();
    m_song_generator->resetPosition();
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
    //m_audioOutput->suspend();
    m_song_generator = 0;
    m_song_generator = new DataGenerator(this);
    connect(m_song_generator, SIGNAL(audioProgressChanged(int)), this, SLOT(on_progressBar_actionTriggered(int)));

    //Prepare the audio device to stream
    m_file->seek(0);
    QByteArray array = m_file->read(44);
    QAudioFormat format = m_song_generator->readHeader(array.data());

    if(!prepare_audio_devices(format))
    {
        qWarning() << "MainWindow::ready_next_song>>Failed preparing the next song";
        if(m_song_generator->isPlaying())
        {
            qDebug() << "Pause button clicked.";
            m_audioOutput->suspend();
        }

        streaming = false;
        song_selected_update(previous);
        move_song_index(previous);
        ready_next_song(previous);
        return false;
    }

    // Always occurs because of delete_old_song();
    if(!fileLoaded)
    {
        qDebug() << "Loading file contents.";
        load_file();
        fileLoaded = true;
    }

    song_selected_update(previous);

    if(!prepare_stream())
    {
        qWarning() << "Prepare stream failed, trying the next song in the list.";
        //Handle streaming prep here.
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
            //std::cerr << "MainWindow::delete_old_song>>Song size: " << *song_size << std::endl;
            ;
        }

        qDebug() << "Disposing of old song.";

        // Required, disconnects old signals and stops sending old data.
        delete m_file;
        m_song_generator->RemoveBufferedData();
        fileLoaded = false;
        fileFinished = false;

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
    return m_music_files.join(' ').toStdString();
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

        //disconnect()
        connect(m_song_generator, SIGNAL(dataAvailable(int)), this, SLOT(handleSongDataAvailable(int)));
        connect(m_song_generator, SIGNAL(dataFinished()), this, SLOT(handleSongDataFinished()));

        qDebug() << "MainWindow::prepare_stream>>Sending Header.";
        QByteArray* header = m_song_generator->getExternalReference();
        header->prepend(HEADER);
        *song_stream_data = header->data();

        //*song_stream_data = m_song_generator->getExternalReference()->data();
        m_stream_size = 51;
        song_size = &m_stream_size;

        return true;
    }

    qDebug() << "MainWindow::prepare_stream>>Header was not sent, streaming set to true when it should be false.";
    return false;
}

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
    if(!recording)
    {
        qDebug() << "recording starts.";
        m_recorder = new Recorder();
        m_voice_generator = new RecordGenerator();

        if(prepare_audio_devices(m_recorder->fileFormat()))
            m_song_generator->setValid();
        else
        {
            qDebug() << "MainWindow::on_recordingButton_clicked>>Can't prepare audio device.";
            return; //Can't set the format properly, too bad.
        }

        connect(m_recorder, SIGNAL(dataAvailable(int)), this, SLOT(handleVoiceDataAvailable(int)));

        m_recorder->start();
        recording = true;
    }
    else
    {
        /*
         * TODO:
         * When button is clicked, turn recording on and off. This will continously "play"
         * the recording until it is clicked off. There will probably be an issue where
         * it will record itself but that's fine.
         */
        m_recorder->stop();
        recording = false;
    }

}

void MainWindow::on_playRecordingButton_clicked()
{
    m_recorder->stop();

    QByteArray array = m_recorder->readAll();

    m_song_generator = new DataGenerator(this);
    m_song_generator->AddMoreDataToBufferFromQByteArray(array, array.size());

    if(prepare_audio_devices(m_recorder->fileFormat()))
        m_song_generator->setValid();

    play_audio();
}

void MainWindow::on_progressBar_actionTriggered(int progress)
{
    ui->progressBar->setValue(progress);
    if(progress == 100)
    {
        streaming = false;
        move_song_index();
        m_audioOutput->suspend();
        m_audioOutput->reset();
        ready_next_song();
    }
}

void MainWindow::handleSongDataAvailable(int len)
{
    *song_size += static_cast <DWORD>(len);
}

void MainWindow::handleVoiceDataAvailable(int len)
{
    char* buf = (char*)malloc(DATA_BUFSIZE);
    //std::cerr << "MainWindow::handleVoiceData>>count:" << cb_voice_data.Count << std::endl;
    if(cb_voice_data.Count != 0)
    {
        CBPop(&cb_voice_data, buf);
        //emit to TCP that this is available

        QByteArray data = QByteArray::fromRawData(buf, len);

        //qDebug() << "Voice size: " << data.size();
        m_voice_generator->AddMoreDataToBufferFromQByteArray(data, data.size());
    }
    play_voice();
    free(buf);
}

void MainWindow::handleSongDataFinished()
{
    qDebug() << "Data has finished sending.";
    fileFinished = true;
    streaming = false;
}

void MainWindow::on_pauseBtn_clicked()
{
    if(!fileExists)
        return;

    if(m_song_generator->isPlaying())
    {
        if(streaming)
        {
            qDebug() << "Pause button clicked.";
            m_audioOutput->suspend();
            streaming = false;
        }
        else {
            streaming = true;
            play_audio();
        }

    }
}

void MainWindow::on_nextsongBtn_clicked()
{
    if(m_song_generator->isPlaying())
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
    if(m_song_generator->isPlaying())
    {
        qDebug() << "Pause button clicked.";
        m_audioOutput->suspend();
    }

    streaming = false;
    move_song_index(true);
    ready_next_song(true);
}
