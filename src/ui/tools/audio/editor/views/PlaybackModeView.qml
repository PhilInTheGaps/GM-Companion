import QtQuick 2.9
import QtQuick.Controls 2.2

import FontAwesome 2.0

Rectangle {
    id: root
    visible: audio_editor.type == 0 || audio_editor.type == 1
    anchors.left: parent.left
    anchors.right: parent.right
    height: color_scheme.toolbarHeight
    color: color_scheme.menuColor

    Row {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 5

        Button {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: height
            hoverEnabled: true

            background: Rectangle {
                color: "transparent"
            }

            ToolTip.text: qsTr("Playlist is shuffled before playing. Loops.")
            ToolTip.visible: hovered

            Text {
                text: FontAwesome.listOl
                font.family: FontAwesome.familySolid
                font.pixelSize: height
                anchors.fill: parent
                anchors.margins: 10
                color: audio_editor.mode == 0 ? "green" : (parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "lightgrey")
            }

            Label {
                text: FontAwesome.random
                font.family: FontAwesome.familySolid
                font.pixelSize: height

                height: parent.height / 3
                width: height
                x: parent.width - width * 1.5
                y: parent.height - height * 1.5

                color: audio_editor.mode == 0 ? "limegreen" : (parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white")

                background: Rectangle {
                    color: color_scheme.menuColor
                }
            }

            onClicked: audio_editor.setMode(audio_editor.name,
                                            audio_editor.type, 0)
        }

        Button {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: height
            hoverEnabled: true

            background: Rectangle {
                color: "transparent"
            }

            ToolTip.text: qsTr("Files are played randomly, does not stop.")
            ToolTip.visible: hovered

            Text {
                text: FontAwesome.random
                font.family: FontAwesome.familySolid
                font.pixelSize: height
                anchors.fill: parent
                anchors.margins: 10
                color: audio_editor.mode == 1 ? "limegreen" : (parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white")
            }

            onClicked: audio_editor.setMode(audio_editor.name,
                                            audio_editor.type, 1)
        }

        Button {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: height
            hoverEnabled: true

            background: Rectangle {
                color: "transparent"
            }

            ToolTip.text: qsTr("Playlist loops in sequential order.")
            ToolTip.visible: hovered

            Text {
                text: FontAwesome.listOl
                font.family: FontAwesome.familySolid
                font.pixelSize: height
                anchors.fill: parent
                anchors.margins: 10
                color: audio_editor.mode == 2 ? "green" : (parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "lightgrey")
            }

            Label {
                text: FontAwesome.sync
                font.family: FontAwesome.familySolid
                font.pixelSize: height

                height: parent.height / 3
                width: height
                x: parent.width - width * 1.5
                y: parent.height - height * 1.5

                color: audio_editor.mode == 2 ? "limegreen" : (parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white")

                background: Rectangle {
                    color: color_scheme.menuColor
                }
            }

            onClicked: audio_editor.setMode(audio_editor.name,
                                            audio_editor.type, 2)
        }

        Button {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: height
            hoverEnabled: true

            background: Rectangle {
                color: "transparent"
            }

            ToolTip.text: qsTr(
                              "Playlist is played in set order. Does not loop.")
            ToolTip.visible: hovered

            Text {
                text: FontAwesome.listOl
                font.family: FontAwesome.familySolid
                font.pixelSize: height
                anchors.fill: parent
                anchors.margins: 10
                color: audio_editor.mode == 3 ? "limegreen" : (parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white")
            }

            onClicked: audio_editor.setMode(audio_editor.name,
                                            audio_editor.type, 3)
        }
    }
}
