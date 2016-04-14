#include "recorder.h"

CircularBuffer cb_voice_data;

Recorder::Recorder()
{
    if( !SetFormat() )
    {
        //CRITICAL ERROR
        inProgress = false;
    }

    r_bytes_AVAILABLE = 0;
    r_buffer.resize(DATA_BUFSIZE);
    CBInitialize(&cb_voice_data, 20, DATA_BUFSIZE);
}

const QAudioFormat &Recorder::fileFormat() const
{
    return r_format;
}

void Recorder::start()
{

    r_inputInfo = QAudioDeviceInfo::defaultInputDevice();

    if(!r_inputInfo.isFormatSupported(r_format))
    {
        r_format = r_inputInfo.nearestFormat(r_format);
    }

    r_input = new QAudioInput(r_inputInfo, r_format, this);

    inProgress = true;
    open(QIODevice::WriteOnly);
    r_input->start(this);
}

void Recorder::stop()
{
    if(r_input != nullptr)
    {
        r_input->stop();
        delete r_input;
    }

    if(inProgress)
    {
        inProgress = false;
    }
}

qint64 Recorder::writeData(const char *data, qint64 len)
{
    qint64 chunk = 0;

    if(r_bytes_AVAILABLE + (int)len > DATA_BUFSIZE)
    {
        chunk = DATA_BUFSIZE - r_bytes_AVAILABLE;

        memcpy(r_buffer.data() + r_bytes_AVAILABLE, data, chunk);

        CBPushBack(&cb_voice_data, r_buffer.data());
        emit dataAvailable(DATA_BUFSIZE);

        r_bytes_AVAILABLE = len - chunk;
        memcpy(r_buffer.data(), data + chunk, len-chunk);
    }
    else
    {
        chunk = len;
        memcpy(r_buffer.data() + r_bytes_AVAILABLE, data, chunk);
        r_bytes_AVAILABLE += len;
    }

    return len;
}

qint64 Recorder::readData(char *data, qint64 maxlen)
{
    Q_UNUSED(data);
    Q_UNUSED(maxlen);

    return ((qint64)0);
}

bool Recorder::SetFormat()
{
    r_format.setSampleRate(16000);
    r_format.setChannelCount(1);
    r_format.setSampleSize(16);
    r_format.setCodec("audio/pcm");
    r_format.setByteOrder(QAudioFormat::LittleEndian);
    r_format.setSampleType(QAudioFormat::UnSignedInt);

    return true;
}

