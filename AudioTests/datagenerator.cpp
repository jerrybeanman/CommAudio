#include "datagenerator.h"

const qint64 ZERO   = 0;

DataGenerator::DataGenerator(QObject *parent, QByteArray *buf)
    :   QIODevice(parent), dg_pos(0), dg_max(0)
{
    playing = false;
    if(dg_pos == 0)
    {

    }
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
    dg_pos = 0;
}

qint64 DataGenerator::readData(char *data, qint64 len)
{
    qint64 total = 0;
    if (!dg_buffer.isEmpty() && playing) {
        while (len - total > 0) {
            const qint64 chunk = qMin((dg_buffer.size() - dg_pos), len - total);
            memcpy(data + total, dg_buffer.constData() + dg_pos, chunk);
            dg_pos = (dg_pos + chunk) % dg_buffer.size();
            total += chunk;
            if(dg_pos == ZERO)
            {
                playing = false;
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

bool DataGenerator::isPlaying()
{
    return playing;
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
