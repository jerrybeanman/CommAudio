#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QByteArray>
#include <atomic>
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
};

#endif // MAINWINDOW_H
