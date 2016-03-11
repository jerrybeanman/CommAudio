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
#include "datagenerator.h"

namespace Ui {

class DataGenerator;

class MainWindow;
}

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

    void on_stopButton_clicked();

    void on_positionChanged(qint64 position);

    void on_durationChanged(qint64 position);

    void begin_pain();

    void prepare_audio_devices();

    void on_pauseButton_clicked();

    void on_openButton_clicked();

    void on_playButton_clicked();

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
    bool                    fileExists;
    bool                    fileLoaded;
};

#endif // AUDIOSTREAM_H
