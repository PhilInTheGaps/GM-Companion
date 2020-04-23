import QtQuick 2.9
import QtQuick.Controls 2.2
import FontAwesome 2.0

AbstractButton {
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

    Row {
        id: row
        anchors.fill: parent

        padding: 5
        spacing: 5

        Label {
            id: icon
            font.pixelSize: (parent.height - parent.padding * 2) * 0.8
            font.family: FontAwesome.familySolid
            anchors.verticalCenter: parent.verticalCenter

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

        Label {
            text: file
            width: parent.width - parent.spacing - parent.padding * 2 - icon.width
            clip: true
            elide: Text.ElideRight
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    hoverEnabled: true

    ToolTip {
        id: tool_tip
        text: type === 3 ? file : qsTr("Add: ") + file
        visible: hovered
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
}
