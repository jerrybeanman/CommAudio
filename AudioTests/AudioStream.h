#ifndef AUDIOSTREAM_H
#define AUDIOSTREAM_H

#include <QMainWindow>
#include <QAudioFormat>
#include <QAudioDecoder>
#include <QMediaPlayer>
#include <QDebug>
#include <QFileDialog>
#include <QBuffer>
#include <QAudioOutput>
#include <QSlider>
#include "wavfile.h"

namespace Ui {

class Generator;

class MainWindow;
}

class DataGenerator : public QIODevice
{
    Q_OBJECT

public:
    DataGenerator(QObject *parent);
    ~DataGenerator();

    void start();
    void stop();

    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
    qint64 bytesAvailable() const;
    void RemoveBufferedData();
    void AddMoreDataToBufferFromFile(QFile* file, qint64 len);
    void AddMoreDataToBufferFromQByteArray(QByteArray array, qint64 len);

private:
    qint64      dg_pos;
    qint64      dg_max;
    QByteArray  dg_buffer;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:

    void handleAudioStateChanged(QAudio::State);

    void on_programSlider_sliderMoved(int position);

    void on_volumeSlider_sliderMoved(int position);

    void on_startButton_clicked();

    void on_stopButton_clicked();

    void on_positionChanged(qint64 position);

    void on_durationChanged(qint64 position);

    void begin_pain(QString filename);

    void on_pushButton_clicked();

    QAudioFormat OptimizeWavFile(QFile* file);

private:
    Ui::MainWindow*         ui;
    QMediaPlayer*           player;
    QByteArray              m_buffer;
    QBuffer*                mediaStream;
    QAudioDecoder*          m_decoder;
    QAudioFormat            m_format;
    QAudioOutput*           m_audioOutput;
    QAudioDeviceInfo        m_device;
    QSlider*                m_volumeSlider;
    WavFile*                m_file;
    DataGenerator*          m_generator;
    char*                   data;
    bool                    m_pullMode;
};

#endif // AUDIOSTREAM_H
