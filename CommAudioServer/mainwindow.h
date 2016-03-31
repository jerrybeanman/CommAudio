#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QByteArray>
#include <QMediaPlayer>
#include <QFileDialog>
#include <QSound>
#include <QBuffer>
#include <QAudioDecoder>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QSlider>
#include <atomic>
#include "wavfile.h"
#include "datagenerator.h"
#include "recorder.h"
#include "soundmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(char** stream, QWidget *parent = 0);
    ~MainWindow();
private slots:

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: on_connectButton_pressed
    --
    -- DATE: March 05, 2015
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE: void on_connectButton_pressed()
    --
    -- RETURNS: void.
    --
    -- NOTES:
    -- Switches the stack widget to the next page.
    ----------------------------------------------------------------------------------------------------------------------*/
    void on_connectButton_pressed();
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: generateSongList
    --
    -- DATE: March 05, 2015
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE: void generateSongList(QByteArray songs)
    --                      songs - list of songs sent by the server
    --
    -- RETURNS: void.
    --
    -- NOTES:
    -- Displays the list of songs to the UI
    ----------------------------------------------------------------------------------------------------------------------*/
    void generatePlaylist(QByteArray songs);

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: on_filePicker_pressed
    --
    -- DATE: March 05, 2015
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE: void on_filePicker_pressed()
    --
    -- RETURNS: void.
    --
    -- NOTES:
    -- Displays a dialog for the user to select a file
    ----------------------------------------------------------------------------------------------------------------------*/
    void on_filePicker_pressed();
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: updateFileProgress
    --
    -- DATE: March 05, 2015
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE: void updateFileProgress(const int progress)
    --                      progress - the progress to update the progress bar with
    --                                  this is a number between 0 and 100
    --
    -- RETURNS: void.
    --
    -- NOTES:
    -- Displays a dialog for the user to select a file
    ----------------------------------------------------------------------------------------------------------------------*/
    void updateFileProgress(const int progress);

private slots:

    void on_pushButton_clicked();

    void on_recordButton_clicked();

    void on_playRecordingButton_clicked();

    void on_progressBar_actionTriggered(int action);

    void on_streamButton_clicked(bool checked);

    void handleDataAvailable(int len);

    void handleDataFinished();

private:
    Ui::MainWindow *ui;
    QByteArray serverIP;
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: getServerAddress
    --
    -- DATE: March 05, 2015
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE: QByteArray getServerAddress()
    --
    -- RETURNS: The IP address entered
    --
    -- NOTES:
    ----------------------------------------------------------------------------------------------------------------------*/
    QByteArray getServerAddress();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: prepare_audio_devices
    --
    -- DATE: March 08, 2015
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Tyler Trepanier-Bracken
    --
    -- PROGRAMMER: Tyler Trepanier-Bracken, Vivek Kalia
    --
    -- INTERFACE: void prepare_audio_devices(QAudioFormat format)
    --
    -- RETURNS: The IP address entered
    --
    -- NOTES:
    ----------------------------------------------------------------------------------------------------------------------*/
    void prepare_audio_devices(QAudioFormat format);

    void load_file();

    void play_audio();

private:
    QMediaPlayer*           player;
    QByteArray              m_buffer;
    QBuffer*                mediaStream;
    QAudioDecoder*          m_decoder;
    QAudioFormat            m_format;
    QAudioOutput*           m_audioOutput;
    QAudioDeviceInfo        m_device;
    QSlider*                m_volumeSlider;
    WavFile*                m_file;
    Recorder*               m_recorder;

    DataGenerator*          m_generator;
    QByteArray*             m_data;
    char**                  m_stream_data;
    DWORD                   m_stream_size;
    qint64                  m_pos;
    bool                    m_pullMode;
    bool                    fileExists;
    bool                    fileLoaded;
    bool                    streaming;
    bool                    fileFinished;
    struct UDPBroadcast*    udp;
};

#endif // MAINWINDOW_H
