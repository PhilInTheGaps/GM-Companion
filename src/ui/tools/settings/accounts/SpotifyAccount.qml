import QtQuick 2.9
import QtQuick.Controls 2.2

Column {
    id: root
    spacing: 10
    anchors.margins: 10
    anchors.left: parent.left
    anchors.right: parent.right

    Connections {
        target: spotify_service

        onWrongPassword: {
            error_label.visible = true
        }
    }

    Text {
        text: qsTr("Spotify")
        font.bold: true
        color: color_scheme.textColor
    }

    Text {
        text: qsTr("(Requires Spotify Premium)")
        color: color_scheme.textColor
    }

    Row {
        spacing: 10

        Text {
            id: username_text
            text: qsTr("Username")
            anchors.verticalCenter: parent.verticalCenter
            color: color_scheme.textColor
            width: client_secret_text.width
        }

        TextField {
            id: usernameTextField
            selectByMouse: true
            width: settings_page.width / 4
            Component.onCompleted: text = settings_tool.getSpotifyUsername()
        }
    }

    Row {
        spacing: 10

        Text {
            text: qsTr("Password")
            width: username_text.width
            anchors.verticalCenter: parent.verticalCenter
            color: color_scheme.textColor
        }

        TextField {
            id: passwordTextField
            selectByMouse: true
            width: settings_page.width / 4
            echoMode: TextField.Password
            Component.onCompleted: text = settings_tool.getSpotifyPassword()
        }
    }

    Text {
        text: qsTr("Connection Settings")
        color: color_scheme.textColor
    }

    RadioButton {
        id: default_server_radio_button
        checked: settings_tool.getServiceConnection() === "default"
        indicator.x: leftPadding
        spacing: 10

        contentItem: Text {
            id: default_server_text
            text: "Use default server"
            color: color_scheme.textColor
            verticalAlignment: Text.AlignVCenter
            leftPadding: default_server_radio_button.indicator.width
                         + default_server_radio_button.spacing
        }
    }

    RadioButton {
        id: custom_server_radio_button
        indicator.x: leftPadding
        indicator.y: custom_server_text.y
        spacing: 10
        checked: settings_tool.getServiceConnection() === "custom"

        contentItem: Column {
            spacing: 10

            Text {
                id: custom_server_text
                text: "Use custom server"
                color: color_scheme.textColor
                verticalAlignment: Text.AlignVCenter
                leftPadding: custom_server_radio_button.indicator.width
                             + custom_server_radio_button.spacing
            }

            Row {
                visible: custom_server_radio_button.checked
                spacing: 10

                Text {
                    text: qsTr("Server URL")
                    width: username_text.width
                    anchors.verticalCenter: parent.verticalCenter
                    color: color_scheme.textColor
                }

                TextField {
                    id: custom_server_textfield
                    selectByMouse: true
                    width: settings_page.width / 4

                    Component.onCompleted: text = settings_tool.getServer()
                }
            }
        }
    }

    RadioButton {
        id: client_id_secret_radio_button
        indicator.x: leftPadding
        indicator.y: client_id_secret_text.y
        spacing: 10
        checked: settings_tool.getServiceConnection() === "local"

        contentItem: Column {
            spacing: 10

            Text {
                id: client_id_secret_text
                text: "Use client id and secret"
                color: color_scheme.textColor
                verticalAlignment: Text.AlignVCenter
                leftPadding: client_id_secret_radio_button.indicator.width
                             + client_id_secret_radio_button.spacing
            }

            Row {
                visible: client_id_secret_radio_button.checked
                spacing: 10

                Text {
                    text: qsTr("Client ID")
                    width: username_text.width
                    anchors.verticalCenter: parent.verticalCenter
                    color: color_scheme.textColor
                }

                TextField {
                    id: spotify_id_textfield
                    selectByMouse: true
                    width: settings_page.width / 4
                    Component.onCompleted: text = settings_tool.getSpotifyID()
                }
            }

            Row {
                visible: client_id_secret_radio_button.checked
                spacing: 10

                Text {
                    id: client_secret_text
                    text: qsTr("Client Secret")
                    anchors.verticalCenter: parent.verticalCenter
                    color: color_scheme.textColor
                }

                TextField {
                    id: spotify_secret_textfield
                    selectByMouse: true
                    width: settings_page.width / 4
                    Component.onCompleted: text = settings_tool.getSpotifySecret()
                }
            }
        }
    }

    Row {
        spacing: 10

        Button {
            text: qsTr("Save & Connect")
            onClicked: {
                error_label.visible = false
                settings_tool.setSpotifyUsername(usernameTextField.text)
                settings_tool.setSpotifyPassword(passwordTextField.text)

                settings_tool.setSpotifySecret(spotify_secret_textfield.text)
                settings_tool.setSpotifyID(spotify_id_textfield.text)

                settings_tool.setServer(custom_server_textfield.text)

                if (default_server_radio_button.checked) {
                    settings_tool.setServiceConnection("default")
                } else if (custom_server_radio_button.checked) {
                    settings_tool.setServiceConnection("custom")
                } else {
                    settings_tool.setServiceConnection("local")
                }

                spotify_service.updateConnector()

                //                spotify_service.startLibrespot()
            }
        }

        Label {
            id: error_label
            color: "white"
            background: Rectangle {
                color: "darkred"
            }

            visible: false
            anchors.verticalCenter: parent.verticalCenter

            text: qsTr("Error: Wrong Password or Username")
        }
    }
}
