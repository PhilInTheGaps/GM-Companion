import QtQuick 2.9
import QtQuick.Controls 2.2

Page {
    id: root

    background: Rectangle {
        color: color_scheme.backgroundColor
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
                    color: color_scheme.textColor
                }

                RadioButton {
                    id: radio_local
                    checked: parent.cloudMode == 0

                    Text {
                        text: qsTr("Local")
                        color: color_scheme.textColor
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                        x: radio_local.indicator.width + 20
                    }

                    onCheckedChanged: {
                        if (checked)
                            settings_tool.setCloudMode(0)
                    }
                }

                RadioButton {
                    id: google_drive_radio_button
                    checked: parent.cloudMode == 1

                    Text {
                        text: qsTr("Google Drive")
                        color: color_scheme.textColor
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                        x: google_drive_radio_button.indicator.width + 20
                    }

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
                        color: color_scheme.textColor
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
                        color: color_scheme.textColor
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
                        text: google_drive_tool.linked ? qsTr("Disconnect") : qsTr(
                                                             "Connect")

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
                    setting: "characters"
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
