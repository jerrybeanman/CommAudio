#include "inputbuffer.h"

#define MAXSIZE 150000

InputBuffer::InputBuffer()
{
    i_buffer.resize(MAXSIZE);
    setBuffer(&i_buffer);

    i_read_pos = 0;
    i_write_pos = 0;
    i_buffered_length = 0;
}

InputBuffer::~InputBuffer()
{

}

qint64 InputBuffer::readData(char *data, qint64 maxlen)
{

    if(i_buffered_length <= 0)
        return 0;

    qint64 writeLen = qMin(maxlen, i_buffered_length);
    if( (writeLen + i_read_pos) > MAXSIZE)
    {
        qint64 firstData = MAXSIZE - i_read_pos;
        memcpy(data, i_buffer.constData() + i_read_pos, firstData);
        memcpy(data+firstData, i_buffer.constData(), writeLen - firstData);
    }
    else
    {
        memcpy(data, i_buffer.constData() + i_read_pos, writeLen);
    }

    i_read_pos = (i_read_pos + writeLen) % MAXSIZE;
    i_buffered_length -= writeLen;

    return writeLen;
}

qint64 InputBuffer::writeData(const char *data, qint64 len)
{
    if((i_write_pos + len) > MAXSIZE)
    {
        qint64 firstData = MAXSIZE - i_write_pos;
        memcpy(i_buffer.data() + i_write_pos, data, firstData);
        memcpy(i_buffer.data(), data+firstData, len - firstData);
    }
    else
    {
        memcpy(i_buffer.data() + i_write_pos, data, len);
    }

    i_write_pos = (i_write_pos + len) % MAXSIZE;
    i_buffered_length += len;

    return len;
}

qint64 InputBuffer::size() const
{
    return i_buffered_length;
}

const QByteArray &InputBuffer::readAll()
{
    i_data.resize(0);
    i_data.resize(i_buffered_length);
    readData(i_data.data(), i_buffered_length);

    return i_data;
}


