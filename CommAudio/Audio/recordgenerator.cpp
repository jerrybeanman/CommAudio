#include "recordgenerator.h"

RecordGenerator::RecordGenerator()
{
    rg_readpos = 0;
    rg_max = 0;
    playing = false;
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

bool RecordGenerator::isPlaying()
{
    return playing;
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
    rg_max = 0;
}

void RecordGenerator::AddMoreDataToBufferFromQByteArray(QByteArray array, qint64 len)
{
    rg_buffer.append(array);
    rg_max += len;
}
