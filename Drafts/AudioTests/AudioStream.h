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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_programSlider_sliderMoved(int position);

    void on_volumeSlider_sliderMoved(int position);

    void on_startButton_clicked();

    void on_stopButton_clicked();

    void on_positionChanged(qint64 position);

    void on_durationChanged(qint64 position);

    void begin_pain(QString filename);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QMediaPlayer* player;
    QByteArray m_buffer;
    QBuffer* mediaStream;
    QAudioDecoder* m_decoder;
    QAudioFormat m_format;
    QAudioOutput* m_audioOutput;
    QAudioDeviceInfo m_device;
    QSlider *m_volumeSlider;
    QFile* m_file;
    bool m_pullMode;
};

#endif // AUDIOSTREAM_H
