import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import FontAwesome 2.0

Rectangle {
    id: file_browser_button

    property string file
    property var path
    property var type
    property bool opened
    property int depth: 0

    signal folderClicked

    anchors.left: parent.left
    anchors.right: parent.right
    anchors.leftMargin: 5 + depth * 10
    anchors.rightMargin: 5
    height: 35

    color: mouse_area.pressed ? "white" : color_scheme.secondaryButtonColor

    Row {
        id: row
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

    MouseArea {
        id: mouse_area
        anchors.fill: parent

        ToolTip {
            id: tool_tip
            text: type === 3 ? file : qsTr("Add: ") + file
        }

        onClicked: {
            if (type === 3) {
                // Is Folder
                audio_editor_file_browser.openFolder(!opened, file, path)
                folderClicked()
            } else {
                audio_editor.addFile(audio_editor.name, audio_editor.type,
                                     path, file)
            }
        }

        hoverEnabled: true

        onEntered: tool_tip.visible = true
        onExited: tool_tip.visible = false
    }
}
