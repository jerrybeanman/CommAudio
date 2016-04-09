#include "datagenerator.h"
const qint64 ZERO   = 0;

DataGenerator::DataGenerator(QObject *parent)
    :   QIODevice(parent), dg_readpos(0), dg_max(0)
{
    playing = false;
    validFormat = false;
}

DataGenerator::~DataGenerator()
{
    dg_buffer.clear();
    dg_buffer.resize(0);
    dg_externBuf.clear();
    dg_externBuf.resize(0);
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
    chunk       descriptor;     // Contains the format of the file.
    quint16     audioFormat;
    quint16     numChannels;
    quint32     sampleRate;
    quint32     byteRate;
    quint16     blockAlign;
    quint16     bitsPerSample;
};

struct DATAHeader
{
    chunk       descriptor; // Contains the format of the file.
};

struct CombinedHeader
{
    RIFFHeader  riff;   // Description of the format type
    WAVEHeader  wave;   // Wave file header
};

QAudioFormat DataGenerator::readHeader(char* data)
{
    QAudioFormat format;
    CombinedHeader header;

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

        validFormat = true;
        return format;
    }
    qDebug() << "READING THE HEADER INSIDE OF THE DATAGENERATOR SCREWED UP";
    validFormat = false;
    return format;
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
    if(!validFormat)
    {
        qDebug() << "DataGenerator::readData>>Invalid Format";
        return chunk;
    }

    if (!dg_buffer.isEmpty() && playing) {

        // Grab the either the length of data requested or the remaining data available.
        chunk = qMin((dg_buffer.size() - dg_readpos), len);
        memcpy(data, dg_buffer.constData() + dg_readpos, chunk);

        externChunk = chunk; // Used to signal when we have data available to output.

        // We do not want the read any more data than our contents.
        dg_readpos = (dg_readpos + chunk) % dg_buffer.size();

        progress = (int)((dg_readpos * 100) / dg_max);

        // Reading has finished.
        if(dg_readpos == ZERO)
        {
            playing = false;
            progress = 100;
            //emit dataFinished();
        }

        //qDebug() << "DataGenerator::readData>>progress[" << progress << "] dataAvailable[" << chunk << "]";
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
    return dg_max;
}

bool DataGenerator::isPlaying()
{
    return playing;
}

void DataGenerator::RemoveBufferedData()
{
    dg_buffer.clear();
    dg_externBuf.clear();
    dg_buffer.resize(0);
    dg_externBuf.resize(0);
    dg_readpos = 0;
    dg_max = 0;
    externChunk = 0;
    progress = 0;
    playing = false;
    validFormat = false;
    close();
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
