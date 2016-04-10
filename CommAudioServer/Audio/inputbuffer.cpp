#include "inputbuffer.h"

#define MAXSIZE 150000

InputBuffer::InputBuffer()
{
    i_size = 0;
    i_busy = false;

}

InputBuffer::~InputBuffer()
{

}

qint64 InputBuffer::readData(char *data, qint64 maxlen)
{
    qint64 chunk = 0;
    while(i_busy)
    {
        ;
    }
    i_busy = true;

    if(maxlen <= i_size)
    {
        chunk = maxlen;
    }
    else
    {
        chunk = i_size;
    }
    memcpy(data, i_buffer.data(), maxlen);
    i_buffer.remove(0, chunk);

    i_size -= chunk;

    i_busy = false;

    return chunk;
}

qint64 InputBuffer::writeData(const char *data, qint64 len)
{
    while(i_busy){
        ;
    }
    i_busy = true;

    i_buffer.resize(i_size + len);
    memcpy(i_buffer.data() + i_size, data, len);

    i_size += len;

    i_busy = false;

    return len;
}

qint64 InputBuffer::size() const
{
    return i_size;
}

const QByteArray &InputBuffer::readAll()
{
    return i_buffer;
}


