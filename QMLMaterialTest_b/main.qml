import QtQuick 2.4
import QtQuick.Layouts 1.1
import Material 0.2
import Material.ListItems 0.1 as ListItem

ApplicationWindow {
    id: demo
    flags: Qt.FramelessWindowHint
    width: 500
    height: 500

    title: "CommAudio"

    // Necessary when loading the window from C++
    visible: true

    property bool hover: false

    theme {
        primaryColor: "#8BC34A"
        accentColor: "white"
        tabHighlightColor: "white"
        backgroundColor: "#CCCCCC"
    }


    property var sectionTitles: [ " " ]

    initialPage: Page {
        id: page
        title: "CommAudio"
    Item {
        anchors.centerIn: parent
        height: column.implicitHeight + Units.dp(60)

        View {
            anchors.centerIn: parent

            width: Units.dp(350)
            height: column.implicitHeight + Units.dp(32)

            elevation: 1
            radius: Units.dp(2)

            ColumnLayout {
                id: column

                anchors {
                    fill: parent
                    topMargin: Units.dp(16)
                    bottomMargin: Units.dp(16)
                }

                Label {
                    id: titleLabel

                    anchors {
                        left: parent.left
                        right: parent.right
                        margins: Units.dp(16)
                    }

                    style: "title"
                    text: "IP Address"
                }

                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: Units.dp(8)
                }

                ListItem.Standard {
                    action: Icon {
                        anchors.centerIn: parent
                        name: "action/account_circle"
                    }

                    content: TextField {
                        id: ipTextField
                        objectName: "ipText"
                        anchors.centerIn: parent
                        width: parent.width

                        text: "192.168.0.1"
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: Units.dp(8)
                }

                RowLayout {
                    Layout.alignment: Qt.AlignRight
                    spacing: Units.dp(8)

                    anchors {
                        right: parent.right
                        margins: Units.dp(16)
                    }

                    Button {
                        text: "Connect"
                        backgroundColor: Theme.primaryColor
                        darkBackground: true
                        textColor: "white"
                        elevation: 1
                        onClicked: connectClick()
                    }
                }
            }
        }
    }

        actions: [

            Action {
                iconName: "image/color_lens"
                name: "Colors"
                onTriggered: colorPicker.show()
                hoverAnimation: true
            },

            Action {
                iconName: "action/settings"
                name: "Settings"
                hoverAnimation: true
                onTriggered: pageStack.push(Qt.resolvedUrl("SidebarPage.qml"))
            }
        ]
      }


    Dialog {
        id: colorPicker
        title: "Pick color"

        MenuField {
            id: selection
            model: ["Primary color", "Accent color", "Background color"]
            width: Units.dp(160)
        }

        Grid {
            columns: 7
            spacing: Units.dp(8)

            Repeater {
                model: [
                    "red", "pink", "purple", "deepPurple", "indigo",
                    "blue", "lightBlue", "cyan", "teal", "green",
                    "lightGreen", "lime", "yellow", "amber", "orange",
                    "deepOrange", "grey", "blueGrey", "brown", "black",
                    "white"
                ]

                Rectangle {
                    width: Units.dp(30)
                    height: Units.dp(30)
                    radius: Units.dp(2)
                    color: Palette.colors[modelData]["500"]
                    border.width: modelData === "white" ? Units.dp(2) : 0
                    border.color: Theme.alpha("#000", 0.26)

                    Ink {
                        anchors.fill: parent

                        onPressed: {
                            switch(selection.selectedIndex) {
                                case 0:
                                    theme.primaryColor = parent.color
                                    break;
                                case 1:
                                    theme.accentColor = parent.color
                                    break;
                                case 2:
                                    theme.backgroundColor = parent.color
                                    break;
                            }
                        }
                    }
                }
            }
        }

        onRejected: {
            // TODO set default colors again but we currently don't know what that is
        }
    }

    function connectClick()
    {
        windowadapter.on_connectButton_pressed(ipTextField.text);
        pageStack.push(Qt.resolvedUrl("SidebarPage.qml"));
        //return ipTextField.text.toString();
    }

    function returnIP()
    {
        //console.log(ipTextField.text.toString());
        return "ipTextField.text.toString()";
    }
}


