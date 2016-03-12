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
    else
    {
        connect(dev, SIGNAL(readyRead()), SLOT(dataReady()));
    }
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

}

QByteArray Recorder::readAll() const
{
    return r_buffer->readAll();
}

int Recorder::bytesWritten()
{
    return r_buffer->size();
}

void Recorder::dataReady()
{
    if(!ready)
    {
        return;
    }

    dev->seek(0);
    r_readyArray = dev->readAll();
    qDebug("[Size:%d]\n", r_readyArray.size());


    /*int blockSize = dev->size(); // returns always zero when using IODevice
    qDebug("%d bytes ready", blockSize);
    if (blockSize <= 0)
        return;

    // 1 Try directly
    r_byteArray = dev->read(blockSize); // <-- not working (ba is empty)
    qDebug("[1] ba size %d",r_byteArray.size() );

    // 2 more or less the same
    r_byteArray = dev->peek(blockSize); // <-- not working (ba is empty)
    qDebug("[2] ba size %d", r_byteArray.size() );

    r_byteArray = dev->readAll();		// fails (ba is empty)
    qDebug("[4] ba size %d", r_byteArray.size() );

    // 5
    dev->read(r_byteArray.data(), r_byteArray.size()); // <-- guess what ?! right: fails!
    qDebug("[5] ba size %d",r_byteArray.size() );*/
}

bool Recorder::SetFormat()
{
    r_format.setSampleRate(8000);
    r_format.setChannelCount(1);
    r_format.setSampleSize(8);
    r_format.setCodec("audio/pcm");
    r_format.setByteOrder(QAudioFormat::LittleEndian);
    r_format.setSampleType(QAudioFormat::UnSignedInt);

    r_input = new QAudioInput(r_format, this);
    if (!bufferFull)
    {
        r_buffer = new QBuffer();
        if (!r_buffer->open(QIODevice::ReadWrite))
        {
            qWarning("unable to open buffer");
            return false;
        }
        //Do I need this?*****
        //r_input->start(r_buffer);
        //dev = r_buffer;
    }
    else
    {
        //Do I need this?****
        //dev = r_input->start();
    }
    ready = true;

    return true;
}
