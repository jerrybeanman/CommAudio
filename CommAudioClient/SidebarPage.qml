/*
 * QML Material - An application framework implementing Material Design.
 * Copyright (C) 2015 Michael Spencer <sonrisesoftware@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 2.1 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
import QtQuick 2.4
import Material 0.2
import Material.ListItems 0.1 as ListItem

TabbedPage {
    id: page
    title: "CommAudio 2.0"

    onGoBack: {
        confirmationDialog.show()
        event.accepted = true
    }

//    rightSidebar: PageSidebar {
//        title: "Playlist"

//        width: Units.dp(250)

//    }

    Tab {
        title: "Streaming"
        iconName: "av/surround_sound"
        Item {
            width: Units.dp(50)
            View {
                anchors {
                    left: parent.left
                    top: parent.top
                    bottom: parent.bottom
                    margins: Units.dp(2)

                }

                elevation: 1
                width: 150
                height: 300

                ListView {
                    id: streamingPlaylistView
                    anchors.fill: parent

                }
            }
            ActionButton {
                anchors {
                    right: parent.right
                    bottom: parent.bottom
                    margins: Units.dp(30)
                }

                action: Action {
                    id: playlistAddContent
                    text: "Add Song"
                    shortcut: "Ctrl+A"
                    onTriggered: snackbar.open("We do actions too!")
                }
                iconName: "content/add"
            }
            Snackbar {
                id: playlistSnackbar
            }
        }
    }

    Tab {
        title: "Microphone"
        iconName: "av/mic"
        Rectangle { color: Palette.colors.purple["200"] }
    }

    Tab {
        title: "Playlist"
        iconName: "notification/live_tv"
        Item {
            width: Units.dp(50)
            View {
                anchors {
                    left: parent.left
                    top: parent.top
                    bottom: parent.bottom
                    margins: Units.dp(2)

                }

                elevation: 1
                width: 150
                height: 300

                ListView {
                    id: playlistView
                    anchors.fill: parent

                }
            }
            ActionButton {
                anchors {
                    right: parent.right
                    bottom: parent.bottom
                    margins: Units.dp(30)
                }

                action: Action {
                    id: addContent
                    text: "Add Song"
                    shortcut: "Ctrl+A"
                    onTriggered: snackbar.open("We do actions too!")
                }
                iconName: "content/add"
            }
            Snackbar {
                id: snackbar
            }
        }
    }



    Dialog {
        id: confirmationDialog

        title: "Disconnect from the server?"
        positiveButtonText: "Disconnect"
        negativeButtonText: "Cancel"

        onAccepted: page.forcePop()
    }
}
