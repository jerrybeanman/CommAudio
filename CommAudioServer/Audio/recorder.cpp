#include "recorder.h"

CircularBuffer cb_voice_data;

Recorder::Recorder()
{
    if( !SetFormat() )
    {
        //CRITICAL ERROR
        inProgress = false;
        ready = false;
    }
    size = 0;
    r_data = new char[DATA_BUFSIZE];
    CBInitialize(&cb_voice_data, 20, DATA_BUFSIZE);
}

Recorder::~Recorder()
{

}

const QAudioFormat &Recorder::fileFormat() const
{
    return r_format;
}

void Recorder::start()
{
    bool status = r_buffer->open(QIODevice::ReadWrite);

    if(!status)
    {
        qDebug() << "Error opening the buffer";
    }

    r_inputInfo = QAudioDeviceInfo::defaultInputDevice();

    if(!r_inputInfo.isFormatSupported(r_format))
    {
        qDebug() <<"Default format not supported, try to use nearest format";
        r_format = r_inputInfo.nearestFormat(r_format);
    }

    r_input = new QAudioInput(r_inputInfo, r_format, this);
    r_input->setNotifyInterval(1000);
    connect(r_input, SIGNAL(notify()), this, SLOT(notified()));
    connect(r_input,SIGNAL(stateChanged(QAudio::State)),this, SLOT(handleAudioInputState(QAudio::State)));

    inProgress = true;
    r_input->start(r_buffer);
}

void Recorder::stop()
{
    if(inProgress)
    {
        r_input->stop();
        delete r_input;
        inProgress = false;
    }
}

qint64 Recorder::readData(char *data, qint64 maxlen)
{
    //qDebug() << "Recorder::readData>>reading";
    qint64 chunk = 0;
    chunk = r_buffer->readData(data, maxlen);
    return chunk;
}

const QByteArray Recorder::readAll()
{
    return r_buffer->readAll();
}

int Recorder::bytesWritten()
{
    return r_buffer->size();
}

void Recorder::notified()
{
    qDebug() << "Notified";
    r_bytes_AVAILABLE = 0;
    if(audio_state == QAudio::ActiveState)
    {
        r_bytes_AVAILABLE = (int)r_buffer->size();

        if(r_bytes_AVAILABLE > DATA_BUFSIZE) // Don't exceed max packet size.
            r_bytes_AVAILABLE = DATA_BUFSIZE;

        readData(r_data, r_bytes_AVAILABLE);

        CBPushBack(&cb_voice_data, (void*)r_data);
        emit dataAvailable(r_bytes_AVAILABLE);
    }
}

void Recorder::handleAudioInputState(QAudio::State state)
{
    //qDebug() << "Audio State:" << state;

    audio_state = state;

    if(state == QAudio::StoppedState)
    {
        qDebug() << "Error State:" << r_input->error();

        if(r_input->error() != QAudio::NoError)
        {
            qDebug() << "QAudioInput error:" << r_input->error();
        }
    }
}

bool Recorder::SetFormat()
{
    r_format.setSampleRate(16000);
    r_format.setChannelCount(1);
    r_format.setSampleSize(16);
    r_format.setCodec("audio/pcm");
    r_format.setByteOrder(QAudioFormat::LittleEndian);
    r_format.setSampleType(QAudioFormat::UnSignedInt);

    r_buffer = new InputBuffer();

    return true;
}

