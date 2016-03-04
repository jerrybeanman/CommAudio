#include "AudioStream.h"
#include "ui_mainwindow.h"

const int DurationSeconds = 1;
const int ToneSampleRateHz = 600;
const int DataSampleRateHz = 44100;
const int BufferSize      = 32768;

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
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::on_startButton_clicked()
{
    //Bad practice, need a check for QFileDialog succeeds or not.
    m_file = 0;

    // Get the file name using a QFileDialog
    m_file = new QFile(QFileDialog::getOpenFileName(this, tr("Upload a file")));
    m_file->open(QIODevice::ReadOnly);
    bool test = (bool)m_file->fileName().isEmpty();
    // If the selected file is valid, continue with the upload
    if (!test) {
        //Skip the wav file header
        m_file->seek(44);

        // Read the file and transform the output to a QByteArray
        m_buffer = m_file->readAll();
        m_file->close();
        mediaStream = new QBuffer(&m_buffer);
        qDebug() << mediaStream->size();
        //begin_pain(NULL);
    }
    /*player->setMedia(QUrl::fromLocalFile("C:/Users/Tyler/Desktop/CommAudio/Drafts/AudioTests/Test.mp3"));
    player->play();
    qDebug() << player->errorString();*/
}

void MainWindow::on_stopButton_clicked()
{
    player->stop();
}

void MainWindow::on_positionChanged(qint64 position)
{
    ui->progressSlider->setValue(position);
}

void MainWindow::on_durationChanged(qint64 position)
{
    ui->progressSlider->setMaximum(position);
}

void MainWindow::begin_pain(QString filename)
{
    m_pullMode = true;

    m_format.setSampleRate(11025);
    m_format.setChannelCount(1);
    m_format.setSampleSize(8);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::UnSignedInt);


    qDebug() << m_device.deviceName();

    /*m_decoder = new QAudioDecoder();
    m_decoder->setAudioFormat(m_format);
    m_decoder->setSourceFilename(filename);*/


    if(!m_device.isFormatSupported(m_format))
    {
        qWarning()<<"raw audio format not supported by backend, cannot play audio.";
        return;
    }

    m_audioOutput = 0;
    m_audioOutput = new QAudioOutput(m_device, m_format, this);

    mediaStream->open(QIODevice::ReadOnly);
    m_audioOutput->start(mediaStream);
    m_audioOutput->setVolume(qreal(100.0f/100.0f));
    //Check the audiooutput project on how it gets outputed.

    /*QEventLoop loop;
    QObject::connect(&output, SIGNAL(stateChanged(QAudio::State)), &loop, SLOT(quit()));
    do {
        loop.exec();
    } while(output.state() == QAudio::ActiveState);*/
}

void MainWindow::on_pushButton_clicked()
{
    begin_pain(NULL);
}
