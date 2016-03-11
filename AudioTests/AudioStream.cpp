#include "AudioStream.h"
#include "recorder.h"
#include "ui_mainwindow.h"

const int DurationSeconds   = 1;
const int ToneSampleRateHz  = 600;
const int DataSampleRateHz  = 44100;
const int BufferSize        = 32768;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this);
    m_device = QAudioDeviceInfo::defaultOutputDevice();
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::on_positionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::on_durationChanged);
    //connect(m_volumeSlider, SIGNAL(on_volumeSlider_sliderMoved(int)), this, SLOT(on_volumeSlider_sliderMoved(int)));
    fileExists = false;
    fileLoaded = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleAudioStateChanged(QAudio::State newState)
{
    /*qDebug() << "State: " << newState;
    switch (newState) {
    case QAudio::IdleState:
        // Finished playing
        m_audioOutput->stop();
        qDebug() << "STOPPED!";
        break;

    case QAudio::StoppedState:
        // Stopped for other
        qDebug() << "STOPPED!";
        if (mAudioOut->error() != QAudio::NoError) {
            // Error handling
            qDebug() << "STOPPED ERROR!";
        }
        break;

    default:
        // ... other cases as appropriate
        qDebug() << "DEFAULT!";
        break;
    }*/
}

void MainWindow::on_programSlider_sliderMoved(int position)
{
    player->setPosition(position);
}

void MainWindow::on_volumeSlider_sliderMoved(int position)
{
    if (m_audioOutput)
        m_audioOutput->setVolume(qreal(position/100.0f));
}

void MainWindow::on_stopButton_clicked()
{
    if(m_generator->isPlaying())
    {
        qDebug() << "Stop button clicked.";
        m_audioOutput->stop();
        m_audioOutput->reset();
        m_generator->resetPosition();
    }
}

void MainWindow::on_positionChanged(qint64 position)
{
    ui->progressSlider->setValue(position); //Not done.
}

void MainWindow::on_durationChanged(qint64 position)
{
    ui->progressSlider->setMaximum(position); //Not done.
}

void MainWindow::begin_pain()
{
    /* Purpose, split the file in half and read it in portions */
    qint64 size = m_file->size() - 44; //Size of the file minus the header.
    QByteArray array = m_file->read(size/2);
    m_generator->AddMoreDataToBufferFromQByteArray(array, size/2);

    array = m_file->read(size/2);
    m_generator->AddMoreDataToBufferFromQByteArray(array, size/2);
}

void MainWindow::prepare_audio_devices()
{
    m_pullMode = true;

    m_format = m_file->fileFormat();
    qDebug() << m_device.deviceName();

    if(!m_device.isFormatSupported(m_format))
    {
        qWarning()<<"raw audio format not supported by backend, cannot play audio.";
        return;
    }

    m_audioOutput = 0;
    m_audioOutput = new QAudioOutput(m_device, m_format, this);

    //connect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleAudioStateChanged(QAudio::State)));
}

void MainWindow::on_pauseButton_clicked()
{
    if(m_generator->isPlaying())
    {
        qDebug() << "Pause button clicked.";
        m_audioOutput->suspend();
    }
}

void MainWindow::on_openButton_clicked()
{
    qDebug() << "Open button clicked.";
    if(fileExists)
    {
        qDebug() << "New file.";
        delete m_file;
        delete m_generator;
        fileLoaded = false;
    }
    m_file = new WavFile(this);
    m_generator = new DataGenerator(this);

    m_file->open(QFileDialog::getOpenFileName(this, tr("Upload a file")));

    prepare_audio_devices();
    fileExists = true;
}

void MainWindow::on_playButton_clicked()
{
    if(!fileLoaded)
    {
        begin_pain();
        fileLoaded = true;
    }

    if(m_generator->isPlaying())
    {
        m_audioOutput->resume();
    }
    else
    {
        m_generator->start();

        m_audioOutput->start(m_generator);
        m_audioOutput->setVolume(qreal(100.0f/100.0f));
    }

}
