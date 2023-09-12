import QtQuick
import QtQuick.Controls
import src
import "../buttons"

Item {
    id: file_browser

    Connections {
        target: AudioTool.editor

        function onCurrentElementChanged() {
            AudioTool.editor.fileBrowser.type
                    = AudioTool.editor.currentElement ? AudioTool.editor.currentElement.type : 0
        }
    }

    ListView {
        anchors.fill: parent
        clip: true
        spacing: 2

        model: AudioTool.editor.fileBrowser.model

        delegate: FileBrowserButton {
            required property AudioEditorFile modelData

            file: modelData ? modelData.name : ""
            type: modelData ? modelData.type : 0
            path: modelData ? modelData.path : []
            depth: modelData ? modelData.depth : 0
            opened: modelData ? modelData.opened : false

            onFolderClicked: modelData.opened = !opened
        }

        ScrollBar.vertical: ScrollBar {}
    }
}
