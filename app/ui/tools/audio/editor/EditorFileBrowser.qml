import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import IconFonts
import "../buttons"

Item {
    id: file_browser

    Connections {
        target: audio_editor

        function onCurrentElementChanged() {
            audio_editor_file_browser.type
                    = audio_editor.currentElement ? audio_editor.currentElement.type : 0
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
