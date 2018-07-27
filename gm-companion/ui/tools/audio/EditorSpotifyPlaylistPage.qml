import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Window 2.2

import gm.companion.colorscheme 1.0

Page {
    id: spotify_page

    signal savePlaylist
    signal changeIcon(string path)
    signal changeID(string id)

    property string resourcesPath

    function setName(name) {
        name_text.text = name
    }

    function setID(id) {
        id_textfield.text = id
    }

    function setIcon(path) {
        icon_textfield.text = path
        icon_image.source = "file://" + resourcesPath + path
    }

    ColorScheme {
        id: color_scheme
    }

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    Column {
        anchors.fill: parent
        spacing: 5

        Text {
            id: name_text
            text: qsTr("Playlist")
            color: color_scheme.textColor
        }

        Row {
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 5

            Button {
                id: save_button
                text: qsTr("Save Playlist")
                onClicked: savePlaylist()
            }

            TextField {
                id: id_textfield
                width: parent.width - parent.spacing - save_button.width
                selectByMouse: true
                placeholderText: qsTr("Playlist ID")

                onTextChanged: changeID(text)
            }
        }

        Text {
            text: qsTr("Custom Icon")
            color: color_scheme.textColor
        }

        TextField {
            id: icon_textfield
            width: parent.width
            selectByMouse: true
            placeholderText: qsTr("Icon Path (if empty spotify thumbnail is used)")

            onTextChanged: {
                changeIcon(text)
                icon_image.source = "file://" + resourcesPath + text
            }
        }

        Image {
            id: icon_image
            width: parent.width / 4
            height: width
        }
    }
}
