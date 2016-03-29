#include "datagenerator.h"

const qint64 ZERO   = 0;

DataGenerator::DataGenerator(QObject *parent)
    :   QIODevice(parent), dg_readpos(0), dg_max(0), dg_streampos(0)
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
            //qDebug() << "DataGenerator::readData>>dataFinished";
            emit dataFinished();
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
