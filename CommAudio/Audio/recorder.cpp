#include "recorder.h"

Recorder::Recorder()
{
    if( !SetFormat() )
    {
        //CRITICAL ERROR
        inProgress = false;
        ready = false;
    }
    size = 0;
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
    bool status = r_newBuffer->open(QIODevice::ReadWrite);

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

    qDebug() << "platform buffer size:" << r_input->bufferSize();

    inProgress = true;
    r_input->start(r_newBuffer);
}

void Recorder::stop()
{
    if(inProgress)
    {
        r_input->stop();
        r_newBuffer->close();
        delete r_input;
        inProgress = false;
    }
}

const QByteArray Recorder::readAll()
{
    return r_newBuffer->readAll();
}

int Recorder::bytesWritten()
{
    return r_newBuffer->size();
}

void Recorder::notified()
{
    if(audio_state == QAudio::ActiveState)
    {
        qDebug() << "Error State:" << r_input->error();
        qDebug() << "Bytes total inputed: " << r_newBuffer->size();
    }
}

void Recorder::handleAudioInputState(QAudio::State state)
{
    qDebug() << "Audio State:" << state;

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
    r_format.setSampleRate(8000);
    r_format.setChannelCount(1);
    r_format.setSampleSize(8);
    r_format.setCodec("audio/pcm");
    r_format.setByteOrder(QAudioFormat::LittleEndian);
    r_format.setSampleType(QAudioFormat::UnSignedInt);

    r_newBuffer = new InputBuffer();

    return true;
}

