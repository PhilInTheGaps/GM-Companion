import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import FontAwesome 2.0

Rectangle {
    id: file_browser_button

    property string file
    property string path
    property var type

    anchors.left: parent.left
    anchors.right: parent.right
    height: 30

    Row {
        anchors.fill: parent
        padding: 5
        spacing: 5

        Text {
            id: icon
            font.pixelSize: (parent.height - parent.padding * 2) * 0.8
            font.family: FontAwesome.familySolid
            anchors.verticalCenter: parent.verticalCenter

            color: mouse_area.pressed ? color_scheme.secondaryButtonColor : "white"

            Component.onCompleted: {
                switch (type) {
                case 0:
                    text = FontAwesome.music
                    break
                case 1:
                    text = FontAwesome.drum
                    break
                case 2:
                    text = FontAwesome.broadcastTower
                    break
                case 3:
                    text = FontAwesome.folder
                    break
                }
            }
        }

        Text {
            text: file
            color: mouse_area.pressed ? "black" : color_scheme.buttonTextColor
            width: parent.width - parent.spacing - parent.padding * 2 - icon.width
            clip: true
            elide: Text.ElideRight
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    color: mouse_area.pressed ? "white" : color_scheme.secondaryButtonColor

    MouseArea {
        anchors.fill: parent
        id: mouse_area

        ToolTip {
            id: tool_tip
            text: type === 3 ? file : "Add: " + file
        }

        onClicked: {
            if (type === 3) {
                audio_editor_file_browser.setCurrentFolder(file)
            } else if (type === 2) {
                audio_editor.addFile(audio_editor.name, audio_editor.type, path)
            } else {
                audio_editor.setFileIndex(file_list.currentIndex)
                audio_editor.addFile(audio_editor.name, audio_editor.type, path)
            }
        }

        hoverEnabled: true

        onEntered: tool_tip.visible = true
        onExited: tool_tip.visible = false
    }
}
