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
#include "Audio/wavfile.h"
#include "Audio/datagenerator.h"
#include "Audio/recorder.h"
#include "soundmanager.h"

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

    void handleDataAvailable(int len);

    void on_pauseBtn_clicked();

    void on_nextsongBtn_clicked();

    void on_prevsongBtn_clicked();

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
    -- RETURNS:     False when the format is not support by the audio device
    --              True when the format is accepted by the audio device
    --
    -- NOTES:
    -- Checks the format parameter and tests to it to see if it will allow the file to work with the default Audio Output
    -- of the listening client. Returns false upon failure
    ----------------------------------------------------------------------------------------------------------------------*/
    bool prepare_audio_devices(QAudioFormat format);

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Load File
    --
    -- DATE:        March 13, 2015
    --
    -- REVISIONS:   (Date and Description)
    --
    -- DESIGNER:    Tyler Trepanier-Bracken
    --
    -- PROGRAMMER:  Tyler Trepanier-Bracken
    --
    -- INTERFACE:   void load_file()
    --
    -- RETURNS:     void
    --
    -- NOTES:
    -- Grabs the file's contents and adds its to the Data Generator's data buffer.
    ----------------------------------------------------------------------------------------------------------------------*/
    void load_file();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Play Audio
    --
    -- DATE:        March 13, 2015
    --
    -- REVISIONS:   (Date and Description)
    --
    -- DESIGNER:    Tyler Trepanier-Bracken
    --
    -- PROGRAMMER:  Tyler Trepanier-Bracken
    --
    -- INTERFACE:   void play_audio()
    --
    -- RETURNS:     void
    --
    -- NOTES:
    -- Checks to see if Audio is currently playing. If it is playing, it is assumed to be paused and will resume playing
    -- the song. Otherwise, this must be a NEW song to be played. As such, it begin to play the new song.
    ----------------------------------------------------------------------------------------------------------------------*/
    void play_audio();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Stop Stream
    --
    -- DATE:        March 17, 2015
    --
    -- REVISIONS:   (Date and Description)
    --
    -- DESIGNER:    Tyler Trepanier-Bracken
    --
    -- PROGRAMMER:  Tyler Trepanier-Bracken
    --
    -- INTERFACE:   void stop_stream()
    --
    -- RETURNS:     void
    --
    -- NOTES:
    -- Stops a currently running stream and drops all references to the stream itself.
    ----------------------------------------------------------------------------------------------------------------------*/
    void stop_stream();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Load Music Files
    --
    -- DATE:        March 25, 2015
    --
    -- REVISIONS:   (Date and Description)
    --
    -- DESIGNER:    Tyler Trepanier-Bracken
    --
    -- PROGRAMMER:  Tyler Trepanier-Bracken
    --
    -- INTERFACE:   void load_music_files()
    --
    -- RETURNS:     void
    --
    -- NOTES:
    -- We are assuming that the directory that contains the music is called "Music" which is located at the root folder
    -- of this program. This grabs all of the contents of the directory of the program and will refresh the song list
    -- if a file was added or removed.
    ----------------------------------------------------------------------------------------------------------------------*/
    void load_music_files();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Move Song Index
    --
    -- DATE:        March 25, 2015
    --
    -- REVISIONS:   (Date and Description)
    --
    -- DESIGNER:    Tyler Trepanier-Bracken
    --
    -- PROGRAMMER:  Tyler Trepanier-Bracken
    --
    -- INTERFACE:   void move_song_index(bool previous = false)
    --
    -- PARAMETERS:  bool previous
    --                  Defaults to false, checks to see if we are changing to the next song or the previous song.
    --
    -- RETURNS:     void
    --
    -- NOTES:
    -- Moves the song index to either the next song or the previous song depending on the boolean: previous.
    ----------------------------------------------------------------------------------------------------------------------*/
    void move_song_index(bool previous = false);

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Ready Next Song
    --
    -- DATE:        March 25, 2016
    --
    -- REVISIONS:   April 4, 2016 (Tyler Trepanier
    --                  Added Stream functionality
    --
    -- DESIGNER:    Tyler Trepanier-Bracken
    --
    -- PROGRAMMER:  Tyler Trepanier-Bracken
    --
    -- INTERFACE:   bool ready_next_song(bool previous = false)
    --
    -- PARAMETERS:  bool previous
    --                  Defaults to false, checks to see if we are changing to the next song or the previous song.
    --
    -- RETURNS:     Returns True if there was no issues with switching to the next song.
    --              Returns False if there was an issue with switching to the next song.
    --
    -- NOTES:
    -- Master function that handles switching between songs for streaming. This function disposes of any old song data,
    -- loads the next song's data to the data generator and begins to initially stream the song to any listening clients.
    ----------------------------------------------------------------------------------------------------------------------*/
    bool ready_next_song(bool previous = false);

    void song_selected_update(bool previous = false);

    bool delete_old_song();

    void populate_songlist();

    std::string get_all_songs();

    void split_songs_from_string(std::string combinedString);

    bool prepare_stream();

private:
    QByteArray              m_buffer;
    QAudioFormat            m_format;
    QAudioOutput*           m_audioOutput;
    QAudioDeviceInfo        m_device;
    QSlider*                m_volumeSlider;
    WavFile*                m_file;
    Recorder*               m_recorder;

    DataGenerator*          m_generator;
    DWORD                   m_stream_size;
    qint64                  m_pos;
    qint64                  m_song_index;
    bool                    fileExists;
    bool                    fileLoaded;
    bool                    streaming;
    bool                    fileFinished;

    QStringList             m_music_files;
    QDir                    m_dir;

};

#endif // MAINWINDOW_H
