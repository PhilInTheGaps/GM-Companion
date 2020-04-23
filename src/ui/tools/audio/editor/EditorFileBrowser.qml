import QtQuick 2.9
import QtQuick.Controls 2.2

import FontAwesome 2.0
import "../buttons"

Item {
    id: file_browser

    Connections {
        target: audio_editor

        onCurrentElementChanged: {
            audio_editor_file_browser.type = audio_editor.type
        }
    }

    ListView {
        anchors.fill: parent
        clip: true
        spacing: 2

        model: audioEditorFileBrowserModel

        delegate: FileBrowserButton {
            file: modelData.name
            type: modelData.type
            path: modelData.path
            depth: modelData.depth
            opened: modelData.opened

            onFolderClicked: modelData.opened = !opened
        }

        ScrollBar.vertical: ScrollBar {}
    }
}
