import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import IconFonts
import common
import services

BaseAccountPage {
    id: root

    name: "Spotify"
    icon: FontAwesome.spotify
    iconFont: FontAwesome.fontBrands
    statuses: [Spotify.status, Spotify.clientStatus]

    leftPanel: Column {
        id: left_panel
        spacing: 10

        function saveConnectionSettings() {
            SettingsManager.spotifyUsername = username_textfield.text
            SettingsManager.setPassword(username_textfield.text,
                                        password_textfield.text, "Spotify")

            SettingsManager.spotifyID = spotify_id_textfield.text
            SettingsManager.spotifySecret = spotify_secret_textfield.text
            SettingsManager.setServerUrl(custom_server_textfield.text,
                                         "Spotify")

            if (default_server_radio_button.checked) {
                SettingsManager.spotifyConnection = "default"
            } else if (custom_server_radio_button.checked) {
                SettingsManager.spotifyConnection = "custom"
            } else {
                SettingsManager.spotifyConnection = "local"
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
            visible: Spotify.connected
            text: qsTr("Username: ") + Spotify.username
            anchors.left: parent.left
            anchors.right: parent.right
            wrapMode: Label.WrapAtWordBoundaryOrAnywhere
        }

        // Login for librespot
        GridLayout {
            visible: !Spotify.connected
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
                Component.onCompleted: text = SettingsManager.spotifyUsername
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
            visible: !Spotify.connected
            spacing: 10
            anchors.left: parent.left
            anchors.right: parent.right

            // Default Server
            RadioButton {
                id: default_server_radio_button
                text: qsTr("Use default server")
                anchors.left: parent.left
                anchors.right: parent.right
                checked: SettingsManager.spotifyConnection === "default"
            }

            // Custom Server
            RadioButton {
                id: custom_server_radio_button
                text: qsTr("Use custom server")
                anchors.left: parent.left
                anchors.right: parent.right
                checked: SettingsManager.spotifyConnection === "custom"
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

                    Component.onCompleted: text = SettingsManager.getServerUrl(
                                               "Spotify", true)
                }
            }

            // Local
            RadioButton {
                id: client_id_secret_radio_button
                text: qsTr("Client ID and Secret")
                anchors.left: parent.left
                anchors.right: parent.right
                checked: SettingsManager.spotifyConnection === "local"
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

                    Component.onCompleted: text = SettingsManager.spotifyID
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

                    Component.onCompleted: text = SettingsManager.spotifySecret
                }
            }
        }

        Button {
            text: Spotify.connected ? qsTr("Disconnect") : qsTr("Connect")

            // Only enable if all required text field are filled out
            enabled: Spotify.connected
                     || (username_textfield.text !== ""
                         && password_textfield.text !== ""
                         && (default_server_radio_button.checked || (custom_server_radio_button.checked && custom_server_textfield.text !== "")
                             || (client_id_secret_radio_button.checked && spotify_id_textfield.text
                                 !== "" && spotify_secret_textfield.text !== "")))

            onClicked: {
                if (Spotify.connected) {
                    Spotify.disconnectService()
                } else {
                    left_panel.saveConnectionSettings()
                    Spotify.connectService()
                }
            }
        }
    }

    rightPanel: Column {
        id: right_panel
        spacing: 10

        function saveQualitySettings() {
            SettingsManager.spotifyEnableCache = enable_audio_cache_checkbox.checked
            SettingsManager.spotifyEnableVolumeNormalization
                    = enable_volume_normalization_checkbox.checked
            SettingsManager.spotifyBitrate = getNewBitrate()
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
            checked: SettingsManager.spotifyEnableCache
        }

        CheckBox {
            id: enable_volume_normalization_checkbox
            text: qsTr("Enable Volume Normalization")
            anchors.left: parent.left
            anchors.right: parent.right
            checked: SettingsManager.spotifyEnableVolumeNormalization
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
                    checked: SettingsManager.spotifyBitrate === "96"
                }

                RadioButton {
                    id: bitrate_button_mid
                    text: qsTr("Mid (160 kbps)")
                    anchors.left: parent.left
                    anchors.right: parent.right
                    checked: SettingsManager.spotifyBitrate === "160"
                }

                RadioButton {
                    id: bitrate_button_high
                    text: qsTr("High (320 kbps)")
                    anchors.left: parent.left
                    anchors.right: parent.right
                    checked: SettingsManager.spotifyBitrate === "320"
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
