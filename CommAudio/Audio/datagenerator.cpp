#include "datagenerator.h"

const qint64 ZERO   = 0;

DataGenerator::DataGenerator()
    :   QIODevice(), dg_readpos(0), dg_max(0), dg_streampos(0)
{
    playing = false;
}

DataGenerator::~DataGenerator()
{

}

void DataGenerator::start()
{
    playing = true;
    open(QIODevice::ReadOnly);
}

void DataGenerator::stop()
{
    qDebug() << "DataGenerator stopped.";
    close();
}

void DataGenerator::resume()
{
    qDebug() << "DataGenerator resumed.";
    open(QIODevice::ReadOnly);
}

void DataGenerator::resetPosition()
{
    playing = false;
    dg_readpos = 0;
}

QByteArray *DataGenerator::getExternalReference()
{
    return &dg_externBuf;
}

qint64 DataGenerator::readData(char *data, qint64 len)
{
    qint64 chunk = 0;
    if (!dg_buffer.isEmpty() && playing) {
        chunk = qMin((dg_buffer.size() - dg_readpos), len);
        memcpy(data, dg_buffer.constData() + dg_readpos, chunk);

        dg_readpos = (dg_readpos + chunk) % dg_buffer.size();

        externChunk = chunk;
        progress = (int)((dg_readpos * 100) / ((qint64)dg_buffer.size()));

        if(dg_readpos == ZERO)
        {
            playing = false;
            progress = 100;
            emit dataFinished();
        }

        emit audioProgressChanged(progress);
        emit dataAvailable(externChunk);
    }
    return chunk;
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

bool DataGenerator::isPlaying()
{
    return playing;
}

void DataGenerator::RemoveBufferedData()
{
    dg_buffer.resize(0);
    dg_externBuf.resize(0);
    dg_readpos = 0;
    dg_max = 0;
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
    dg_externBuf.resize(dg_max + len);
    dg_buffer.append(array);
    memcpy(dg_externBuf.data() + dg_max, dg_buffer.constData() + dg_max, len);
    dg_max += len;
}



struct chunk
{
    char        id[4];
    quint32     size;
};

struct RIFFHeader
{
    chunk       descriptor;     // "RIFF"
    char        type[4];        // "WAVE"
};

struct WAVEHeader
{
    chunk       descriptor;
    quint16     audioFormat;
    quint16     numChannels;
    quint32     sampleRate;
    quint32     byteRate;
    quint16     blockAlign;
    quint16     bitsPerSample;
};

struct DATAHeader
{
    chunk       descriptor;
};

struct CombinedHeader
{
    RIFFHeader  riff;
    WAVEHeader  wave;
};

QAudioFormat DataGenerator::readHeader(char* data)
{
    QAudioFormat format;
    CombinedHeader header;
    bool result = false;

    memcpy(reinterpret_cast<char *>(&header), data, sizeof(CombinedHeader));
    data += sizeof(WAVEHeader);

    if ((memcmp(&header.riff.descriptor.id, "RIFF", 4) == 0
        || memcmp(&header.riff.descriptor.id, "RIFX", 4) == 0)
        && memcmp(&header.riff.type, "WAVE", 4) == 0
        && memcmp(&header.wave.descriptor.id, "fmt ", 4) == 0
        && (header.wave.audioFormat == 1 || header.wave.audioFormat == 0))
    {

        // Skip extra remaining header information
        DATAHeader dataHeader;

        if (qFromLittleEndian<quint32>(header.wave.descriptor.size) > sizeof(WAVEHeader)) {
            size_t bytes_to_SKIP = qFromLittleEndian<quint32>(header.wave.descriptor.size) % sizeof(WAVEHeader);
            data += bytes_to_SKIP;
        }

        memcpy((char*)(&dataHeader), data, sizeof(DATAHeader));


        // Establish format
        if (memcmp(&header.riff.descriptor.id, "RIFF", 4) == 0)
            format.setByteOrder(QAudioFormat::LittleEndian);
        else
            format.setByteOrder(QAudioFormat::BigEndian);

        int bps = qFromLittleEndian<quint16>(header.wave.bitsPerSample);
        format.setChannelCount(qFromLittleEndian<quint16>(header.wave.numChannels));
        format.setCodec("audio/pcm");
        format.setSampleRate(qFromLittleEndian<quint32>(header.wave.sampleRate));
        format.setSampleSize(qFromLittleEndian<quint16>(header.wave.bitsPerSample));
        format.setSampleType(bps == 8 ? QAudioFormat::UnSignedInt : QAudioFormat::SignedInt);

        qDebug() << "Successfully read the file header";

        return format;
    }
    qDebug() << "READING THE HEADER INSIDE OF THE DATAGENERATOR SCREWED UP";
    return format;
}
