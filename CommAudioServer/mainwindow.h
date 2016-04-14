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
#include "Audio/recordgenerator.h"
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

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Get All Songs
    --
    -- DATE:		April 9th, 2016             REVISIONS:
    --
    -- DESIGNER:	Tyler Trepanier				PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	void get_all_songs();
    --
    -- RETURNS: void
    --
    -- NOTES: Populates the global song list with the current song list.
    --------------------------------------------------------------------------------------------------------------------*/
    void get_all_songs();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:	Get Current Song
    --
    -- DATE:		April 9th, 2016             REVISIONS:
    --
    -- DESIGNER:	Tyler Trepanier				PROGRAMMER:	Tyler Trepanier
    --
    -- INTERFACE:	void get_current_song();
    --
    -- RETURNS: void
    --
    -- NOTES: Resets the global CurrentSong to be the current song that is playing.
    --------------------------------------------------------------------------------------------------------------------*/
    void get_current_song();
    ~MainWindow();
private slots:

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

    void on_progressBar_actionTriggered(int action);

    void handleSongDataAvailable(int len);

    void handleVoiceDataAvailable(int len);

    void handleSongDataFinished();

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
    -- FUNCTION:    Play Voice
    --
    -- DATE:        March 13, 2015
    --
    -- REVISIONS:   (Date and Description)
    --
    -- DESIGNER:    Tyler Trepanier-Bracken
    --
    -- PROGRAMMER:  Tyler Trepanier-Bracken
    --
    -- INTERFACE:   void play_voice()
    --
    -- RETURNS:     void
    --
    -- NOTES:
    -- Checks to see if Voice is currently playing. If it is playing, it is assumed to be paused and will resume playing
    -- the song. Otherwise, this must be a NEW song to be played. As such, it begin to play the new song.
    ----------------------------------------------------------------------------------------------------------------------*/
    void play_voice();

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

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Selected Song Update
    --
    -- DATE:        April 4, 2016
    --
    -- DESIGNER:    Tyler Trepanier-Bracken
    --
    -- PROGRAMMER:  Tyler Trepanier-Bracken
    --
    -- INTERFACE:   void song_selected_update(bool previous = false)
    --
    -- PARAMETERS:  bool previous
    --                  Defaults to false, checks to see if we are changing to the next song or the previous song.
    --
    -- RETURNS:     void
    --
    -- NOTES:
    -- Updates the colours of the last song that was being played using the boolean value.
    ----------------------------------------------------------------------------------------------------------------------*/
    void song_selected_update(bool previous = false);

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Delete old song
    --
    -- DATE:        April 4, 2016
    --
    -- DESIGNER:    Tyler Trepanier-Bracken
    --
    -- PROGRAMMER:  Tyler Trepanier-Bracken
    --
    -- INTERFACE:   bool delete_old_song();
    --
    -- PARAMETERS:  void
    --
    -- RETURNS:     True if there was no issue removing the old song contents. False, otherwise.
    --
    -- NOTES:
    -- Attempts to delete the contents of the old song from the data generator.
    ----------------------------------------------------------------------------------------------------------------------*/
    bool delete_old_song();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Populate Songlist
    --
    -- DATE:        April 4, 2016
    --
    -- DESIGNER:    Tyler Trepanier-Bracken
    --
    -- PROGRAMMER:  Tyler Trepanier-Bracken
    --
    -- INTERFACE:   void populate_songlist();
    --
    -- PARAMETERS:  void
    --
    -- RETURNS:     void
    --
    -- NOTES:
    -- Fills up the play list with any new songs.
    ----------------------------------------------------------------------------------------------------------------------*/
    void populate_songlist();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Splits Song From String
    --
    -- DATE:        April 4, 2016
    --
    -- DESIGNER:    Tyler Trepanier-Bracken
    --
    -- PROGRAMMER:  Tyler Trepanier-Bracken
    --
    -- INTERFACE:   void split_songs_from_string(std::string combinedString);
    --
    -- PARAMETERS:  std::string combinedString
    --                  String to parse and combine into a QStringList.
    --
    -- RETURNS:     void
    --
    -- NOTES:
    -- Splits strings received and sets the Song List to newly parsed list.
    ----------------------------------------------------------------------------------------------------------------------*/
    void split_songs_from_string(std::string combinedString);

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Prepare Song Header
    --
    -- DATE:        April 10th, 2016
    --
    -- DESIGNER:    Tyler Trepanier-Bracken
    --
    -- PROGRAMMER:  Tyler Trepanier-Bracken
    --
    -- INTERFACE:   void prepare_song_header();
    --
    -- PARAMETERS:  void
    --
    -- RETURNS:     void
    --
    -- NOTES:
    -- Prepares the song header listening to the stream by grabbing the first 500 bytes and sending it to all of the
    -- clients.
    ----------------------------------------------------------------------------------------------------------------------*/
    void prepare_song_header();

    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION:    Prepare Stream
    --
    -- DATE:        April 4, 2016
    --
    -- DESIGNER:    Tyler Trepanier-Bracken
    --
    -- PROGRAMMER:  Tyler Trepanier-Bracken
    --
    -- INTERFACE:   bool prepare_stream();
    --
    -- PARAMETERS:  void
    --
    -- RETURNS:     True if we can prepare the stream for streaming a song; false otherwise.
    --
    -- NOTES:
    -- Before the song is broadcasted, we send the song header and the current song being played to all of the clients
    -- via the TCP Control channel.
    ----------------------------------------------------------------------------------------------------------------------*/
    bool prepare_stream();

private:
    QAudioFormat            m_format;           // Song playback format
    QAudioOutput*           m_audioOutput;      // Current default output device
    QAudioDeviceInfo        m_device;           // Checks the playback format for validity
    WavFile*                m_file;             // Holds the current song
    Recorder*               m_recorder;         // Voice recorder that transmits voice data

    DataGenerator*          m_song_generator;   // Data Generator that holds all of the song data
    RecordGenerator*        m_voice_generator;  // Voice data container that plays mic data
    DWORD                   m_stream_size;      // Handles the quantity of song data to send at a time
    qint64                  m_song_index;       // Current song index in the song list
    bool                    fileExists;         // Flag to verify that we have a song to play
    bool                    fileLoaded;         // Flag to veriy that the file loaded properly
    bool                    streaming;          // Flag to indicate that we are streaming
    bool                    recording;          // Flag to indicate that WE are recording
    bool                    fileFinished;       // Flag to indicate that the song file is finished

    QStringList             m_music_files;      // Current song list
    QDir                    m_dir;              // Current directory

};

#endif // MAINWINDOW_H
