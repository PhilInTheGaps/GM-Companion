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
            text: qsTr("Username")
            width: client_secret_text.width
            anchors.verticalCenter: parent.verticalCenter
            color: color_scheme.textColor
        }

        TextField {
            id: usernameTextField
            selectByMouse: true
            onTextChanged: settings_tool.setSpotifyUsername(text)
            width: settings_page.width / 4

            Component.onCompleted: text = settings_tool.getSpotifyUsername()
        }
    }

    Row {
        spacing: 10

        Text {
            text: qsTr("Password")
            width: client_secret_text.width
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

    Row {
        spacing: 10

        Button {
            text: qsTr("Save")
            onClicked: {
                error_label.visible = false
                settings_tool.setSpotifyUsername(usernameTextField.text)
                settings_tool.setSpotifyPassword(passwordTextField.text)
                spotify_service.openSpotify()
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

    Text {
        text: qsTr("Application Settings")
        color: color_scheme.textColor
    }

    Row {
        spacing: 10

        Text {
            text: qsTr("Client ID")
            width: client_secret_text.width
            anchors.verticalCenter: parent.verticalCenter
            color: color_scheme.textColor
        }

        TextField {
            selectByMouse: true
            onTextChanged: settings_tool.setSpotifyID(text)
            width: settings_page.width / 4

            Component.onCompleted: text = settings_tool.getSpotifyID()
        }
    }

    Row {
        spacing: 10

        Text {
            id: client_secret_text
            text: qsTr("Client Secret")
            anchors.verticalCenter: parent.verticalCenter
            color: color_scheme.textColor
        }

        TextField {
            selectByMouse: true
            onTextChanged: settings_tool.setSpotifySecret(text)
            width: settings_page.width / 4

            Component.onCompleted: text = settings_tool.getSpotifySecret()
        }
    }
}
