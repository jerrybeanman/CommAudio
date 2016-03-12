#include "recorder.h"

Recorder::Recorder()
{
    if( !SetFormat() )
    {
        //CRITICAL ERROR
        inProgress = false;
        ready = false;
        bufferFull = false;
    }
    writeBuffer = 1;
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
    bool status1 = r_saveBuffer->open(QIODevice::ReadWrite);

    if(!status || !status1)
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
    r_input->start(r_newBuffer);
}

void Recorder::stop()
{
    r_input->stop();
    r_readyArray.resize(r_newBuffer->size());

    r_newBuffer->seek(0);
    r_readyArray = r_newBuffer->readAll();

    r_newBuffer->close();
    qDebug() << "Ready array size: " << r_readyArray.size();
    delete r_input;
}

QByteArray Recorder::readAll() const
{
    return r_readyArray;
}

int Recorder::bytesWritten()
{
    return r_newBuffer->size();
}

void Recorder::swapBuffers()
{

}

void Recorder::notified()
{
    if(audio_state == QAudio::ActiveState)
    {
        qDebug() << "Error State:" << r_input->error();
        qDebug() << "Bytes total inputed: " << r_newBuffer->size();
        qDebug() << "platform buffer size after called QAudioInput start():" << r_input->bufferSize();

        qDebug() << "bytesReady = " << r_input->bytesReady()
        << ", " << "elapsedUSecs = " << r_input->elapsedUSecs()
        << ", " << "processedUSecs = "<< r_input->processedUSecs();

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

    r_newBuffer = new QBuffer();
    r_saveBuffer = new QBuffer();

    return true;
}
