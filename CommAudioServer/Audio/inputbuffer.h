#ifndef INPUTBUFFER_H
#define INPUTBUFFER_H

#include <QBuffer>
#include <QByteArray>
#include "globals.h"


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
    qint64      i_size;
    bool        i_busy;
};

#endif // INPUTBUFFER_H
