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
#include <QThread>
#include "Audio/recorder.h"
#include "Audio/datagenerator.h"
#include "Audio/wavfile.h"
#include "udpthreadmanager.h"
#include "circularbuffer.h"
#include "tcpthreadmanager.h"

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
    -- FUNCTION: tabSelected
    --
    -- DATE: March 18, 2015
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE: void tabSelected()
    --
    -- RETURNS: void.
    --
    -- NOTES:
    -- Determines what tab has been switched to
    ----------------------------------------------------------------------------------------------------------------------*/
    void tabSelected();

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
    void generatePlaylist(const QByteArray& songs);

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

    void on_recordButton_clicked();

    void on_playRecordingButton_clicked();

    void on_progressBar_actionTriggered(int action);


    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: addToSongBuffer
    --
    -- DATE: March 20, 2015
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE: void addToSongBuffer(const unsigned int size)
    --                              size: Size of the song data to pass to the audio player
    --
    -- RETURNS: void.
    --
    -- NOTES:
    ----------------------------------------------------------------------------------------------------------------------*/
    void addToSongBuffer(const unsigned int size);

    void addToSongHeader(const unsigned int size);


    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: on_play_clicked
    --
    -- DATE: March 05, 2015
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE: void on_play_clicked()
    --
    -- RETURNS: void.
    --
    -- NOTES:
    -- Displays a dialog for the user to select a file and play a file
    ----------------------------------------------------------------------------------------------------------------------*/
    void on_play_clicked();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: on_requestFile_clicked
    --
    -- DATE: March 05, 2015
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE: void on_requestFile_clicked()
    --
    -- RETURNS: void.
    --
    -- NOTES:
    -- Sends the file which was hilighted in the list of songs
    ----------------------------------------------------------------------------------------------------------------------*/
    void on_requestFile_clicked();

private:
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
    -- RETURNS: void
    --
    -- NOTES:
    ----------------------------------------------------------------------------------------------------------------------*/
    void prepare_audio_devices(QAudioFormat format);



    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: initializeUDPThread
    --
    -- DATE: March 08, 2015
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE: void initializeUDPThread()
    --
    -- RETURNS: void
    --
    -- NOTES:
    ----------------------------------------------------------------------------------------------------------------------*/
    void initializeUDPThread();

    void initializeMicrophoneConnection();

    void init_file();

    void play_audio();

    Ui::MainWindow*         ui;
    QByteArray              serverIP;
    QMediaPlayer*           player;
    QByteArray              m_buffer;
    QBuffer*                mediaStream;
    QAudioDecoder*          m_decoder;
    QAudioFormat            m_format;
    QAudioOutput*           m_audioOutput = 0;
    QAudioDeviceInfo        m_device;
    QSlider*                m_volumeSlider;
    WavFile*                m_file;
    Recorder*               m_recorder;
    QThread*                broadcastThread;
    QThread*                tcpThread;
    QFile*                  data_file;
    UDPThreadManager*       UDPWorker;
    TCPThreadManager*       TCPWorker;

    DataGenerator*          m_generator;
    char*                   data;
    bool                    m_pullMode;
    bool                    fileExists;
    bool                    fileLoaded;

    enum tabs {broadcasting, fileTransfer, mic};
};

#endif // MAINWINDOW_H
