#include "AudioStream.h"
#include "ui_mainwindow.h"

const int DurationSeconds   = 1;
const int ToneSampleRateHz  = 600;
const int DataSampleRateHz  = 44100;
const int BufferSize        = 32768;
const qint64 ZERO           = 0;

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

void MainWindow::on_startButton_clicked()
{

    m_file = new WavFile(this);
    m_file->open(QFileDialog::getOpenFileName(this, tr("Upload a file")));

    delete m_generator;
    m_generator = new DataGenerator(this);
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

    m_format = m_file->fileFormat();
    qDebug() << m_device.deviceName();

    if(!m_device.isFormatSupported(m_format))
    {
        qWarning()<<"raw audio format not supported by backend, cannot play audio.";
        return;
    }

    m_audioOutput = 0;
    m_audioOutput = new QAudioOutput(m_device, m_format, this);

    connect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleAudioStateChanged(QAudio::State)));

    /* Purpose, split the file in half and read it in portions */
    qint64 size = m_file->size() - 44; //Size of the file minus the header.
    QByteArray array = m_file->read(size/2);
    m_generator->AddMoreDataToBufferFromQByteArray(array, size/2);

    array = m_file->read(size/2);
    m_generator->AddMoreDataToBufferFromQByteArray(array, size/2);

    m_generator->start();

    m_audioOutput->start(m_generator);
    m_audioOutput->setVolume(qreal(100.0f/100.0f));
}

void MainWindow::on_pushButton_clicked()
{
    begin_pain(NULL);
}

DataGenerator::DataGenerator(QObject *parent)
    :   QIODevice(parent), dg_pos(0), dg_max(0)
{
    if(dg_pos == 0)
    {

    }
}

DataGenerator::~DataGenerator()
{

}

void DataGenerator::start()
{
    open(QIODevice::ReadOnly);
}

void DataGenerator::stop()
{
    dg_pos = 0;
    close();
}


qint64 DataGenerator::readData(char *data, qint64 len)
{
    qint64 total = 0;
    if (!dg_buffer.isEmpty()) {
        while (len - total > 0) {
            const qint64 chunk = qMin((dg_buffer.size() - dg_pos), len - total);
            memcpy(data + total, dg_buffer.constData() + dg_pos, chunk);
            dg_pos = (dg_pos + chunk) % dg_buffer.size();
            total += chunk;
            if(dg_pos == ZERO)
            {
                RemoveBufferedData();
                break;
            }
        }
    }
    return total;
}

qint64 DataGenerator::writeData(const char *data, qint64 len)
{
    //No writing allowed.
    Q_UNUSED(data);
    Q_UNUSED(len);

    return 0;
}

qint64 DataGenerator::bytesAvailable() const
{
    return dg_buffer.size() + QIODevice::bytesAvailable();
}

void DataGenerator::RemoveBufferedData()
{
    dg_buffer.resize(0);
    dg_pos = 0;
}

/*
 *  Adds in Data from a file.
 */
void DataGenerator::AddMoreDataToBufferFromFile(QFile *file, qint64 len)
{
    if(file->isOpen())
    {
        dg_max += len;
        dg_buffer.resize(dg_max);
        dg_buffer = file->read(len);
    }

}

void DataGenerator::AddMoreDataToBufferFromQByteArray(QByteArray array, qint64 len)
{
    dg_max += len;
    dg_buffer.append(array);
}
























//Garbage function, trying to grab header info... no worky
QAudioFormat MainWindow::OptimizeWavFile(QFile* file)
{
    // Read in the whole thing
    QByteArray wavFileContent = file->readAll();

    qDebug() << "The size of the WAV file is: " << wavFileContent.size();

    // Define the header components
    char fileType[4];
    qint32 fileSize;
    char waveName[4];
    char fmtName[3];
    qint32 fmtLength;
    short fmtType;
    short numberOfChannels;
    qint32 sampleRate;
    qint32 sampleRateXBitsPerSampleXChanngelsDivEight;
    short bitsPerSampleXChannelsDivEightPointOne;
    short bitsPerSample;
    char dataHeader[4];
    qint32 dataSize;

    // Create a data stream to analyze the data
    QDataStream analyzeHeaderDS(&wavFileContent,QIODevice::ReadOnly);
    analyzeHeaderDS.setByteOrder(QDataStream::LittleEndian);


    // Now pop off the appropriate data into each header field defined above
    analyzeHeaderDS.readRawData(fileType,4); // "RIFF"
    analyzeHeaderDS >> fileSize; // File Size
    analyzeHeaderDS.readRawData(waveName,4); // "WAVE"
    analyzeHeaderDS.readRawData(fmtName,3); // "fmt"
    analyzeHeaderDS >> fmtLength; // Format length
    analyzeHeaderDS >> fmtType; // Format type
    analyzeHeaderDS >> numberOfChannels; // Number of channels
    analyzeHeaderDS >> sampleRate; // Sample rate
    analyzeHeaderDS >> sampleRateXBitsPerSampleXChanngelsDivEight; // (Sample Rate * BitsPerSample * Channels) / 8
    analyzeHeaderDS >> bitsPerSampleXChannelsDivEightPointOne; // (BitsPerSample * Channels) / 8.1
    analyzeHeaderDS >> bitsPerSample; // Bits per sample
    analyzeHeaderDS.readRawData(dataHeader,4); // "data" header
    analyzeHeaderDS >> dataSize; // Data Size

    // Print the header
    qDebug() << "WAV File Header read:";
    qDebug() << "File Type: " << QString::fromUtf8(fileType);
    qDebug() << "File Size: " << fileSize;
    qDebug() << "WAV Marker: " << QString::fromUtf8(waveName);
    qDebug() << "Format Name: " << QString::fromUtf8(fmtName);
    qDebug() << "Format Length: " << fmtLength;
    qDebug() << "Format Type: " << fmtType;
    qDebug() << "Number of Channels: " << numberOfChannels;
    qDebug() << "Sample Rate: " << sampleRate;
    qDebug() << "Sample Rate * Bits/Sample * Channels / 8: " << sampleRateXBitsPerSampleXChanngelsDivEight;
    qDebug() << "Bits per Sample * Channels / 8.1: " << bitsPerSampleXChannelsDivEightPointOne;
    qDebug() << "Bits per Sample: " << bitsPerSample;
    qDebug() << "Data Header: " << QString::fromUtf8(dataHeader);
    qDebug() << "Data Size: " << dataSize;

    QAudioFormat nope;

    return nope;
}
