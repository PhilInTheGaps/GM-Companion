import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0

Item {
    id: root
    readonly property int textfieldWidth: width / 3

    Column {
        spacing: 10
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: status_label.visible ? status_label.top : parent.bottom

        Row {
            spacing: 5

            Label {
                text: FontAwesome.spotify
                font.family: FontAwesome.familyBrands
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
            }

            Label {
                text: "Spotify"
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        Label {
            text: qsTr("(Requires Spotify Premium)")
        }

        // If connected
        Grid {
            visible: spotify_service.connected
            columns: 2
            spacing: 10
            verticalItemAlignment: Grid.AlignVCenter

            Label {
                text: qsTr("Username")
            }

            Label {
                text: spotify_service.username
            }
        }

        // If not connected
        Grid {
            visible: !spotify_service.connected
            columns: 2
            spacing: 10
            verticalItemAlignment: Grid.AlignVCenter

            // Username
            Label {
                text: qsTr("Username")
            }

            TextField {
                id: username_textfield
                selectByMouse: true
                width: textfieldWidth
                Component.onCompleted: text = settings_manager.getSetting(
                                           "spotifyUsername", "", "Spotify")
            }

            Label {
                text: qsTr("Password")
            }

            TextField {
                id: password_textfield
                selectByMouse: true
                width: textfieldWidth
                echoMode: TextField.Password
            }
        }

        Grid {
            // Password
            columns: 2
            spacing: 10
            verticalItemAlignment: Grid.AlignVCenter

            // Default Server
            RadioButton {
                id: default_server_radio_button
                text: qsTr("Use default server")
                enabled: !spotify_service.connected
                checked: settings_manager.getSetting("connection", "default",
                                                     "Spotify") === "default"
            }

            Item {
                width: 5
                height: 5
            }

            // Custom Server
            RadioButton {
                id: custom_server_radio_button
                text: qsTr("Use custom server")
                enabled: !spotify_service.connected
                checked: settings_manager.getSetting("connection", "default",
                                                     "Spotify") === "custom"
            }

            Item {
                width: 5
                height: 5
            }

            Label {
                visible: custom_server_radio_button.checked
                         && !spotify_service.connected
                text: qsTr("Server URL")
            }

            TextField {
                id: custom_server_textfield
                selectByMouse: true
                width: textfieldWidth
                visible: custom_server_radio_button.checked
                         && !spotify_service.connected

                Component.onCompleted: text = settings_manager.getServerUrl(
                                           "Spotify")
            }

            // Local
            RadioButton {
                id: client_id_secret_radio_button
                text: qsTr("Client ID and Secret")
                enabled: !spotify_service.connected
                checked: settings_manager.getSetting("connection", "default",
                                                     "Spotify") === "local"
            }

            Item {
                width: 5
                height: 5
            }

            Label {
                text: qsTr("Client ID")
                visible: client_id_secret_radio_button.checked
                         && !spotify_service.connected
            }

            TextField {
                id: spotify_id_textfield
                selectByMouse: true
                width: textfieldWidth
                visible: client_id_secret_radio_button.checked
                         && !spotify_service.connected
                Component.onCompleted: text = settings_manager.getSetting(
                                           "spotifyID", "", "Spotify")
            }

            Label {
                text: qsTr("Client Secret")
                visible: client_id_secret_radio_button.checked
                         && !spotify_service.connected
            }

            TextField {
                id: spotify_secret_textfield
                selectByMouse: true
                width: textfieldWidth
                visible: client_id_secret_radio_button.checked
                         && !spotify_service.connected
                Component.onCompleted: text = settings_manager.getSetting(
                                           "spotifySecret", "", "Spotify")
            }
        }

        Button {
            text: spotify_service.connected ? qsTr("Disconnect") : qsTr(
                                                  "Connect")
            onClicked: {
                if (spotify_service.connected) {
                    spotify_service.disconnectService()
                } else {
                    settings_manager.setSetting("spotifyUsername",
                                                username_textfield.text,
                                                "Spotify")
                    settings_manager.setPassword(username_textfield.text,
                                                 password_textfield.text,
                                                 "Spotify")

                    settings_manager.setSetting("spotifyID",
                                                spotify_id_textfield.text,
                                                "Spotify")
                    settings_manager.setSetting("spotifySecret",
                                                spotify_secret_textfield.text,
                                                "Spotify")
                    settings_manager.setSetting("server",
                                                custom_server_textfield.text,
                                                "Spotify")

                    if (default_server_radio_button.checked) {
                        settings_manager.setSetting("connection", "default",
                                                    "Spotify")
                    } else if (custom_server_radio_button.checked) {
                        settings_manager.setSetting("connection", "custom",
                                                    "Spotify")
                    } else {
                        settings_manager.setSetting("connection", "local",
                                                    "Spotify")
                    }

                    spotify_service.connectService()
                }
            }
        }
    }

    StatusBar {
        id: status_label
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        type: spotify_service.status.type
        text: spotify_service.status.message
    }
}
