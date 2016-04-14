/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: mainwindow.h - GUI file used to communicate with the back end and front end
--
-- PROGRAM: commAudio
--
-- FUNCTIONS:
-- explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void on_connectButton_pressed();
    void tabSelected();
    void generatePlaylist(const QByteArray& songs);
    void on_progressBar_actionTriggered(int action);
    void addToSongBuffer(const unsigned int size);
    void addSongHeader();
    void setSong(const QByteArray name);
    void on_play_clicked();
    void on_requestFile_clicked();
    void handleVoiceDataAvailable(const unsigned int);
    void on_peerConnect_clicked();
    void on_volumeSlider_valueChanged(int value);
    QByteArray getServerAddress();
    void prepare_audio_devices(QAudioFormat format);
    void initializeUDPThread();
    void initializeMicrophoneConnection()
    void init_file();
    void play_audio();
    void play_voice();
--
--
-- DATE: April 13, 2016
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Scott Plummer
--
-- PROGRAMMER: Scott Plummer, Tyler Trepanier-Bracken, Vivek Kalia
--
-- NOTES:
-- The client side of the comm audio project. The client side receives music from the multicasted server, it can request
-- a file and play it locally, and connect to a peer and
----------------------------------------------------------------------------------------------------------------------*/
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
#include <QMutex>
#include <QThread>
#include "Audio/recorder.h"
#include "Audio/datagenerator.h"
#include "Audio/wavfile.h"
#include "udpthreadmanager.h"
#include "microphonethreadmanager.h"
#include "microphonethreadrecievingmanager.h"
#include "circularbuffer.h"
#include "tcpthreadmanager.h"
#include "ui_mainwindow.h"
#include "Audio/recordgenerator.h"

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

private slots:

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

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: addToSongHeader
    --
    -- DATE: March 20, 2015
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE: void addToSongBuffer()
    --
    -- RETURNS: void.
    --
    -- NOTES:
    ----------------------------------------------------------------------------------------------------------------------*/
    void addSongHeader();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: setSong
    --
    -- DATE: April 13, 2015
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE: void setSong(const QByteArray name)
    --                          name: Name of the current song
    --
    -- RETURNS: void.
    --
    -- NOTES:
    ----------------------------------------------------------------------------------------------------------------------*/
    void setSong(const QByteArray name);

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

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: Handle Voice Data Available
    --
    -- DATE: April 11, 2015
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Tyler Trepanier
    --
    -- PROGRAMMER: Tyler Trepanier
    --
    -- INTERFACE: void handleVoiceDataAvailable(const unsigned int)
    --
    -- RETURNS: void.
    --
    -- NOTES:
    -- Handles any incoming microphone data and populates the m_voice_generator with this data
    ----------------------------------------------------------------------------------------------------------------------*/
    void handleVoiceDataAvailable(const unsigned int);

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: on_peerConnect_clicked
    --
    -- DATE: March 05, 2015
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE: void on_peerConnect_clicked();
    --
    -- RETURNS: void.
    --
    -- NOTES:
    -- Starts the microphone communications
    ----------------------------------------------------------------------------------------------------------------------*/
    void on_peerConnect_clicked();

    void on_volumeSlider_valueChanged(int value);

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

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: initializeMicrophoneConnection
    --
    -- DATE: March 08, 2015
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE: void initializeMicrophoneConnection()
    --
    -- RETURNS: void
    --
    -- NOTES: connects the micorphone signals to slots
    ----------------------------------------------------------------------------------------------------------------------*/
    void initializeMicrophoneConnection();

    void init_file();

    void play_audio();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: Play Voice
    --
    -- DATE: April 11, 2016
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Tyler Trepanier
    --
    -- PROGRAMMER: Tyler Trepanier
    --
    -- INTERFACE: void play_voice()
    --
    -- RETURNS: void
    --
    -- NOTES:
    -- Reads from the m_voice_generator and plays the microphone data received from the peer.
    ----------------------------------------------------------------------------------------------------------------------*/
    void play_voice();

    Ui::MainWindow*         ui;
    QByteArray              serverIP;
    QByteArray              peerIP;
    QMutex                  test;
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
    QThread*                microphoneThread;
    QThread*                microphoneRecvThread;
    QFile*                  data_file;
    UDPThreadManager*       UDPWorker = nullptr;
    TCPThreadManager*       TCPWorker;
    MicrophoneThreadRecvManager* microphoneRecvWorker;
    MicrophoneThreadManager* microphoneWorker = nullptr;

    DataGenerator*          m_generator;
    RecordGenerator*        m_voice_generator = nullptr;
    char*                   data;
    bool                    recording = false;
    bool                    fileExists;
    bool                    fileLoaded;
    bool                    ReceivingVoice;
    bool                    headerReceived;
    int                     volume;
    enum tabs {broadcasting, fileTransfer, mic};
};

#endif // MAINWINDOW_H
