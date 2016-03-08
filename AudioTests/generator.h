#ifndef GENERATOR_H
#define GENERATOR_H

#include <math.h>
#include <QFileDialog>
#include <QAudioOutput>
#include <QByteArray>
#include <QComboBox>
#include <QIODevice>
#include <QLabel>
#include <QMainWindow>
#include <QObject>
#include <QPushButton>
#include <QSlider>
#include <QTimer>
#include <QFile>
#include <QMediaPlayer>

#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <QDebug>
#include <QBuffer>
#include <QVBoxLayout>
#include <qmath.h>
#include <qendian.h>

class Generator : public QIODevice
{
    Q_OBJECT

public:
    Generator();
    ~Generator();

    void start();
    void stop();

    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
    qint64 bytesAvailable() const;

private:
    void generateData(const QAudioFormat &format, qint64 durationUs, int sampleRate);

private:
    qint64 m_pos;
    QByteArray m_buffer;
    QBuffer *buffer;
    QAudioFormat m_format;
    QAudioOutput *m_audioOutput;
};

#endif // GENERATOR_H
