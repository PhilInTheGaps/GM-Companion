import QtQuick 2.9
import QtQuick.Controls 2.2
import QtWebEngine 1.5

Page {
    id: root

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    Connections {
        target: google_drive_tool

        onOpenWebsite: {
            web_view.url = url
            web_view.reload()
            web_dialog.open()
        }

        onLinkStatusChanged: {
            if (google_drive_tool.linked) {
                web_dialog.close()
            }
        }
    }

    Dialog {
        id: web_dialog

        width: parent.width * 0.75
        height: parent.height * 0.75

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        contentItem: Item {

            WebEngineView {
                id: web_view
                onUrlChanged: console.log(url)

                anchors.fill: parent
            }
        }

        onOpened: console.log("OPENED CLOUD AUTH DIALOG")
    }

    contentItem: Item {
        anchors.fill: parent

        ScrollView {
            id: left_scroll
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: parent.width / 3
            clip: true
            padding: 10
            contentWidth: -1

            Column {
                spacing: 10
                anchors.left: parent.left
                anchors.right: parent.right

                property int cloudMode: settings_tool.getCloudMode()

                Label {
                    text: qsTr("Select File Storage")
                    font.bold: true
                }

                RadioButton {
                    text: qsTr("Local")
                    checked: parent.cloudMode == 0

                    onCheckedChanged: {
                        if (checked)
                            settings_tool.setCloudMode(0)
                    }
                }

                RadioButton {
                    id: google_drive_radio_button
                    text: qsTr("Google Drive")
                    checked: parent.cloudMode == 1

                    onCheckedChanged: {
                        if (checked)
                            settings_tool.setCloudMode(1)
                    }
                }

                Text {
                    text: qsTr("Google Drive")
                    font.bold: true
                    color: color_scheme.textColor
                }

                Row {
                    spacing: 10

                    Text {
                        id: google_id_text
                        text: qsTr("Client ID")
                        width: google_secret_text.width
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    TextField {
                        id: google_id_field
                        text: settings_tool.getGoogleID()
                        selectByMouse: true
                        onTextChanged: {
                            google_drive_tool.setClientID(text)
                        }
                    }
                }

                Row {
                    id: google_secret_row
                    spacing: 10

                    Text {
                        id: google_secret_text
                        text: qsTr("Client Secret")
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    TextField {
                        id: google_secret_field
                        text: settings_tool.getGoogleSecret()
                        selectByMouse: true
                        onTextChanged: {
                            google_drive_tool.setClientSecret(text)
                        }
                    }
                }

                Row {
                    spacing: 10

                    Button {
                        id: google_connect_button
                        text: qsTr("Connect")

                        onClicked: {
                            if (settings_tool.getGoogleConnect()) {
                                google_drive_tool.unlink()
                            } else {
                                google_drive_tool.link()
                            }

                            settings_tool.setGoogleConnect(
                                        !settings_tool.getGoogleConnect())
                        }
                    }

                    Rectangle {
                        id: google_status_rect

                        width: google_secret_row.width - parent.spacing
                               - google_connect_button.width
                        height: google_secret_field.height

                        Text {
                            anchors.centerIn: parent
                            text: qsTr("Status:") + " "
                                  + google_drive_tool.linked ? qsTr("Connected") : qsTr(
                                                                   "Not Connected")
                        }
                    }
                }
            }
        }

        ScrollView {
            anchors.left: left_scroll.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            clip: true
            contentWidth: -1
            visible: google_drive_radio_button.checked

            Column {
                id: path_fields_column
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10
                spacing: 10

                CloudPathItem {
                    path_type: qsTr("Audio Projects")
                    setting: "audio"
                }

                CloudPathItem {
                    path_type: qsTr("Music")
                    setting: "music"
                }

                CloudPathItem {
                    path_type: qsTr("Sound")
                    setting: "sounds"
                }

                CloudPathItem {
                    path_type: qsTr("Internet Radio Playlists")
                    setting: "radio"
                }

                CloudPathItem {
                    path_type: qsTr("Maps")
                    setting: "maps"
                }

                CloudPathItem {
                    path_type: qsTr("Characters")
                    setting: "chracters"
                }

                CloudPathItem {
                    path_type: qsTr("Notes")
                    setting: "notes"
                }

                CloudPathItem {
                    path_type: qsTr("Shop Projects / Item Library")
                    setting: "shop"
                }

                CloudPathItem {
                    path_type: qsTr("Resources")
                    setting: "resources"
                }
            }
        }
    }
}
