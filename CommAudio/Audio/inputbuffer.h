#ifndef INPUTBUFFER_H
#define INPUTBUFFER_H

#include <QBuffer>
#include <QByteArray>


class InputBuffer : public QBuffer
{
public:
    InputBuffer();

    ~InputBuffer();

    qint64 readData(char * data, qint64 maxlen);

    qint64 writeData(const char *data, qint64 len);

    qint64 size() const;

    const QByteArray& readAll();

private:
    QByteArray  i_buffer;
    QByteArray  i_data;             //used for reading all of the data
    qint64      i_read_pos;
    qint64      i_write_pos;
    qint64      i_buffered_length;
};

#endif // INPUTBUFFER_H
