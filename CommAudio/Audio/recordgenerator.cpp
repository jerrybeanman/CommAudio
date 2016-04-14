#include "recordgenerator.h"

RecordGenerator::RecordGenerator()
{
    rg_readpos = 0;
    rg_max = 0;
    playing = false;
    rg_buffer.clear();
    rg_buffer.resize(0);
}

void RecordGenerator::start()
{
    playing = true;
    open(QIODevice::ReadOnly);
}

void RecordGenerator::stop()
{
    //qDebug() << "RecordGenerator stopped.";
    close();
}

void RecordGenerator::resume()
{
    //qDebug() << "DataGenerator resumed.";
    open(QIODevice::ReadOnly);
}

void RecordGenerator::resetData()
{
    rg_readpos = rg_max;
}

bool RecordGenerator::isPlaying()
{
    return playing;
}

qint64 RecordGenerator::bytesAvailable() const
{
    return rg_max;
}

qint64 RecordGenerator::readData(char *data, qint64 maxlen)
{
    qint64 chunk = 0;

    if (!rg_buffer.isEmpty() && playing) {

        // Grab the either the length of data requested or the remaining data available.
        chunk = qMin((rg_buffer.size() - rg_readpos), maxlen);
        memcpy(data, rg_buffer.constData() + rg_readpos, chunk);

        // We do not want the read any more data than our contents.
        rg_readpos = (rg_readpos + chunk) % rg_buffer.size();

        //qDebug() << "RecordGenerator::readData>>dataAvailable[" << chunk << "]";

        RemoveBufferedData(chunk);
        qDebug() << "Data size:" << rg_buffer.size();

    }

    return chunk;
}

qint64 RecordGenerator::writeData(const char *data, qint64 len)
{
    //No writing allowed.
    Q_UNUSED(data);
    Q_UNUSED(len);

    return 0;
}

void RecordGenerator::RemoveBufferedData(int len)
{
    rg_buffer.remove(0, len);
    rg_readpos = 0;
    rg_max -= len;
}

void RecordGenerator::AddMoreDataToBufferFromQByteArray(QByteArray array, qint64 len)
{
    qDebug() << "Added data";
    rg_buffer.append(array);
    rg_max += len;
}
