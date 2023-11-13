import QtQuick
import QtQuick.Controls
import IconFonts
import CustomComponents
import src
import common
import "../../../../.."

Rectangle {
    id: root

    anchors.left: parent.left
    anchors.right: parent.right
    height: Sizes.toolbarHeight
    border.color: Colors.border
    border.width: 1
    color: palette.window

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
            iconColor: AudioTool.editor.currentElement && AudioTool.editor.currentElement.mode === 0 ? SettingsManager.colors.success : palette.text

            Label {
                text: FontAwesome.shuffle
                font.family: FontAwesome.fontSolid.family
                font.styleName: FontAwesome.fontSolid.styleName
                font.pixelSize: height

                height: parent.height / 3
                width: height
                x: parent.width - width * 1.5
                y: parent.height - height * 1.5

                color: AudioTool.editor.currentElement && AudioTool.editor.currentElement.mode === parent.modeNum ? SettingsManager.colors.success : (parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white")

                background: Rectangle {
                    radius: 5
                    color: palette.alternateBase
                }
            }

            onClicked: AudioTool.editor.currentElement.mode = modeNum
        }

        CustomToolBarButton {
            property int modeNum: 1
            toolTipText: qsTr("Files are played randomly, does not stop.")
            iconText: FontAwesome.shuffle
            pointSize: 18
            iconColor: AudioTool.editor.currentElement && AudioTool.editor.currentElement.mode === modeNum ? SettingsManager.colors.success : palette.text

            onClicked: AudioTool.editor.currentElement.mode = modeNum
        }

        CustomToolBarButton {
            property int modeNum: 2

            toolTipText: qsTr("Playlist loops in sequential order.")
            iconText: FontAwesome.listOl
            pointSize: 18
            iconColor: AudioTool.editor.currentElement && AudioTool.editor.currentElement.mode === 2 ? SettingsManager.colors.success : palette.text

            Label {
                text: FontAwesome.rotate
                font.family: FontAwesome.fontSolid.family
                font.styleName: FontAwesome.fontSolid.styleName
                font.pixelSize: height

                height: parent.height / 3
                width: height
                x: parent.width - width * 1.5
                y: parent.height - height * 1.5

                color: AudioTool.editor.currentElement && AudioTool.editor.currentElement.mode === parent.modeNum ? SettingsManager.colors.success : (parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white")

                background: Rectangle {
                    radius: 5
                    color: palette.alternateBase
                }
            }

            onClicked: AudioTool.editor.currentElement.mode = modeNum
        }

        CustomToolBarButton {
            property int modeNum: 3
            toolTipText: qsTr("Playlist is played in set order. Does not loop.")
            iconText: FontAwesome.listOl
            pointSize: 18
            iconColor: AudioTool.editor.currentElement && AudioTool.editor.currentElement.mode === modeNum ? SettingsManager.colors.success : palette.text

            onClicked: AudioTool.editor.currentElement.mode = modeNum
        }
    }
}
