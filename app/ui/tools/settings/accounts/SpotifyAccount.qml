import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.1
import CustomComponents 1.0
import FontAwesome 2.0

BaseAccountPage {
    id: root

    name: "Spotify"
    icon: FontAwesome.spotify
    iconFamily: FontAwesome.familyBrands
    status: [spotify_service.status, spotify_service.clientStatus]

    leftPanel: Column {
        id: left_panel
        spacing: 10

        function saveConnectionSettings() {
            settings_manager.setSetting("spotifyUsername",
                                        username_textfield.text, "Spotify")
            settings_manager.setPassword(username_textfield.text,
                                         password_textfield.text, "Spotify")

            settings_manager.setSetting("spotifyID", spotify_id_textfield.text,
                                        "Spotify")
            settings_manager.setSetting("spotifySecret",
                                        spotify_secret_textfield.text,
                                        "Spotify")
            settings_manager.setSetting("server", custom_server_textfield.text,
                                        "Spotify")

            if (default_server_radio_button.checked) {
                settings_manager.setSetting("connection", "default", "Spotify")
            } else if (custom_server_radio_button.checked) {
                settings_manager.setSetting("connection", "custom", "Spotify")
            } else {
                settings_manager.setSetting("connection", "local", "Spotify")
            }
        }

        Label {
            text: qsTr("(Requires Spotify Premium)")
            anchors.left: parent.left
            anchors.right: parent.right
            wrapMode: Label.WrapAtWordBoundaryOrAnywhere
        }

        // if connected, show username
        Label {
            visible: spotify_service.connected
            text: qsTr("Username: ") + spotify_service.username
            anchors.left: parent.left
            anchors.right: parent.right
            wrapMode: Label.WrapAtWordBoundaryOrAnywhere
        }

        // Login for librespot
        GridLayout {
            visible: !spotify_service.connected
            columns: 2
            columnSpacing: 10
            anchors.left: parent.left
            anchors.right: parent.right

            Label {
                id: username_label
                text: qsTr("Username")
                Layout.alignment: Qt.AlignVCenter
            }

            TextField {
                id: username_textfield
                selectByMouse: true
                Layout.fillWidth: true
                Component.onCompleted: text = settings_manager.getSetting(
                                           "spotifyUsername", "", "Spotify")
            }

            Label {
                id: password_label
                text: qsTr("Password")
                Layout.alignment: Qt.AlignVCenter
            }

            TextField {
                id: password_textfield
                selectByMouse: true
                Layout.fillWidth: true
                echoMode: TextField.Password
            }
        }

        // Connection settings
        Column {
            visible: !spotify_service.connected
            spacing: 10
            anchors.left: parent.left
            anchors.right: parent.right

            // Default Server
            RadioButton {
                id: default_server_radio_button
                text: qsTr("Use default server")
                anchors.left: parent.left
                anchors.right: parent.right
                checked: settings_manager.getSetting("connection", "default",
                                                     "Spotify") === "default"
            }

            // Custom Server
            RadioButton {
                id: custom_server_radio_button
                text: qsTr("Use custom server")
                anchors.left: parent.left
                anchors.right: parent.right
                checked: settings_manager.getSetting("connection", "default",
                                                     "Spotify") === "custom"
            }

            GridLayout {
                columns: 2
                columnSpacing: 10
                anchors.left: parent.left
                anchors.right: parent.right
                visible: custom_server_radio_button.checked

                Label {
                    id: server_url_label
                    text: qsTr("Server URL")
                    Layout.alignment: Qt.AlignVCenter
                }

                TextField {
                    id: custom_server_textfield
                    selectByMouse: true
                    Layout.fillWidth: true

                    Component.onCompleted: text = settings_manager.getServerUrl(
                                               "Spotify")
                }
            }

            // Local
            RadioButton {
                id: client_id_secret_radio_button
                text: qsTr("Client ID and Secret")
                anchors.left: parent.left
                anchors.right: parent.right
                checked: settings_manager.getSetting("connection", "default",
                                                     "Spotify") === "local"
            }

            GridLayout {
                columns: 2
                columnSpacing: 10
                anchors.left: parent.left
                anchors.right: parent.right
                visible: client_id_secret_radio_button.checked

                Label {
                    id: client_id_label
                    text: qsTr("Client ID")
                    Layout.alignment: Qt.AlignVCenter
                }

                TextField {
                    id: spotify_id_textfield
                    selectByMouse: true
                    Layout.fillWidth: true

                    Component.onCompleted: text = settings_manager.getSetting(
                                               "spotifyID", "", "Spotify")
                }

                Label {
                    id: client_secret_label
                    text: qsTr("Client Secret")
                    Layout.alignment: Qt.AlignVCenter
                }

                TextField {
                    id: spotify_secret_textfield
                    selectByMouse: true
                    Layout.fillWidth: true

                    Component.onCompleted: text = settings_manager.getSetting(
                                               "spotifySecret", "", "Spotify")
                }
            }
        }

        Button {
            text: spotify_service.connected ? qsTr("Disconnect") : qsTr(
                                                  "Connect")

            // Only enable if all required text field are filled out
            enabled: spotify_service.connected
                     || (username_textfield.text !== ""
                         && password_textfield.text !== ""
                         && (default_server_radio_button.checked || (custom_server_radio_button.checked && custom_server_textfield.text !== "")
                             || (client_id_secret_radio_button.checked && spotify_id_textfield.text
                                 !== "" && spotify_secret_textfield.text !== "")))

            onClicked: {
                if (spotify_service.connected) {
                    spotify_service.disconnectService()
                } else {
                    left_panel.saveConnectionSettings()
                    spotify_service.connectService()
                }
            }
        }
    }

    rightPanel: Column {
        id: right_panel
        spacing: 10

        function saveQualitySettings() {
            settings_manager.setSetting("enableCache",
                                        enable_audio_cache_checkbox.checked,
                                        "Spotify")
            settings_manager.setSetting(
                        "enableVolumeNormalization",
                        enable_volume_normalization_checkbox.checked, "Spotify")
            settings_manager.setSetting("bitrate", getNewBitrate(), "Spotify")
        }

        function getNewBitrate() {
            if (bitrate_button_low.checked)
                return "96"
            if (bitrate_button_mid.checked)
                return "160"
            if (bitrate_button_high.checked)
                return "320"
        }

        Label {
            text: qsTr("Quality Settings")
        }

        CheckBox {
            id: enable_audio_cache_checkbox
            text: qsTr("Enable Audio Cache")
            anchors.left: parent.left
            anchors.right: parent.right
            checked: settings_manager.getBoolSetting("enableCache", true,
                                                     "Spotify")
        }

        CheckBox {
            id: enable_volume_normalization_checkbox
            text: qsTr("Enable Volume Normalization")
            anchors.left: parent.left
            anchors.right: parent.right
            checked: settings_manager.getBoolSetting(
                         "enableVolumeNormalization", false, "Spotify")
        }

        Column {
            spacing: 10
            anchors.left: parent.left
            anchors.right: parent.right

            Label {
                text: qsTr("Bitrate")
                Layout.alignment: Qt.AlignTop
            }

            Column {
                anchors.left: parent.left
                anchors.right: parent.right

                RadioButton {
                    id: bitrate_button_low
                    text: qsTr("Low (96 kbps)")
                    anchors.left: parent.left
                    anchors.right: parent.right
                    checked: settings_manager.getSetting("bitrate", "160",
                                                         "Spotify") === "96"
                }

                RadioButton {
                    id: bitrate_button_mid
                    text: qsTr("Mid (160 kbps)")
                    anchors.left: parent.left
                    anchors.right: parent.right
                    checked: settings_manager.getSetting("bitrate", "160",
                                                         "Spotify") === "160"
                }

                RadioButton {
                    id: bitrate_button_high
                    text: qsTr("High (320 kbps)")
                    anchors.left: parent.left
                    anchors.right: parent.right
                    checked: settings_manager.getSetting("bitrate", "160",
                                                         "Spotify") === "320"
                }
            }
        }

        Button {
            text: qsTr("Save")

            onClicked: {
                right_panel.saveQualitySettings()
            }
        }
    }
}
