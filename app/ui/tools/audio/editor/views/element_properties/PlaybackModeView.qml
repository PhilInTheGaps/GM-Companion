import QtQuick 2.9
import QtQuick.Controls 2.2
import FontAwesome 2.0
import CustomComponents 1.0
import "../../../../../sizes.js" as Sizes
import "../../../../../colors.js" as Colors

Rectangle {
    id: root

    anchors.left: parent.left
    anchors.right: parent.right
    height: Sizes.toolbarHeight
    border.color: Colors.border
    border.width: 1
    color: Colors.window

    Row {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 5

        CustomToolBarButton {
            property int modeNum: 0
            toolTipText: qsTr("Playlist is shuffled before playing. Loops.")
            iconText: FontAwesome.listOl
            pointSize: 18
            iconColor: audio_editor && audio_editor.currentElement
                       && audio_editor.currentElement.mode === 0 ? "green" : Colors.text

            Label {
                text: FontAwesome.random
                font.family: FontAwesome.familySolid
                font.pixelSize: height

                height: parent.height / 3
                width: height
                x: parent.width - width * 1.5
                y: parent.height - height * 1.5

                color: audio_editor && audio_editor.currentElement.mode
                       === parent.modeNum ? "limegreen" : (parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white")

                background: Rectangle {
                    radius: 5
                    color: Colors.alternateBase
                }
            }

            onClicked: audio_editor.currentElement.mode = modeNum
        }

        CustomToolBarButton {
            property int modeNum: 1
            toolTipText: qsTr("Files are played randomly, does not stop.")
            iconText: FontAwesome.random
            pointSize: 18
            iconColor: audio_editor && audio_editor.currentElement
                       && audio_editor.currentElement.mode === modeNum ? "green" : Colors.text

            onClicked: audio_editor.currentElement.mode = modeNum
        }

        CustomToolBarButton {
            property int modeNum: 2

            toolTipText: qsTr("Playlist loops in sequential order.")
            iconText: FontAwesome.listOl
            pointSize: 18
            iconColor: audio_editor && audio_editor.currentElement
                       && audio_editor.currentElement.mode === 2 ? "green" : Colors.text

            Label {
                text: FontAwesome.sync
                font.family: FontAwesome.familySolid
                font.pixelSize: height

                height: parent.height / 3
                width: height
                x: parent.width - width * 1.5
                y: parent.height - height * 1.5

                color: audio_editor && audio_editor.currentElement.mode
                       === parent.modeNum ? "limegreen" : (parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white")

                background: Rectangle {
                    radius: 5
                    color: Colors.alternateBase
                }
            }

            onClicked: audio_editor.currentElement.mode = modeNum
        }

        CustomToolBarButton {
            property int modeNum: 3
            toolTipText: qsTr("Playlist is played in set order. Does not loop.")
            iconText: FontAwesome.listOl
            pointSize: 18
            iconColor: audio_editor && audio_editor.currentElement
                       && audio_editor.currentElement.mode === modeNum ? "green" : Colors.text

            onClicked: audio_editor.currentElement.mode = modeNum
        }
    }
}
