import QtQuick 2.9
import QtQuick.Controls 2.2
import FontAwesome 2.0
import CustomComponents 1.0

CustomButton {
    id: file_browser_button

    property string file
    property var path
    property var type
    property bool opened
    property int depth: 0

    signal folderClicked

    anchors.left: parent ? parent.left : undefined
    anchors.right: parent ? parent.right : undefined
    anchors.leftMargin: 5 + depth * 10
    anchors.rightMargin: 5
    height: 30

    pointSize: 10
    buttonText: file
    iconText: {
        switch (type) {
        case 0:
            FontAwesome.music
            break
        case 1:
            FontAwesome.drum
            break
        case 2:
            FontAwesome.broadcastTower
            break
        case 3:
            opened ? FontAwesome.folderOpen : FontAwesome.folder
            break
        }
    }

    toolTipText: type === 3 ? file : qsTr("Add: ") + file

    onClicked: {
        if (type === 3) {
            // Is Folder
            audio_editor_file_browser.openFolder(!opened, file, path)
            folderClicked()
        } else {
            audio_editor.addFile(path, file)
        }
    }
}
