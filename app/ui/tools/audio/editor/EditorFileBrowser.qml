import QtQuick
import QtQuick.Controls
import src
import common
import "../buttons"

Item {
    id: file_browser

    Connections {
        target: AudioTool.editor

        function onCurrentElementChanged() {
            AudioTool.editor.fileBrowser.type = AudioTool.editor.currentElement ? AudioTool.editor.currentElement.type : 0;
        }
    }

    ListView {
        id: list_view
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

        ScrollBar.vertical: ScrollBar {
        }
    }

    Label {
        id: empty_label
        visible: AudioTool.editor.fileBrowser.model.isEmpty
        anchors.fill: parent
        padding: 5

        readonly property string path: switch (AudioTool.editor.fileBrowser.type) {
        case 0:
            return SettingsManager.getPath("music");
        case 1:
            return SettingsManager.getPath("sounds");
        case 2:
            return SettingsManager.getPath("radio");
        default:
            return "";
        }

        text: qsTr("No files available.\nThe folder is:\n\n") + path + "\n\nThis can be changed in the settings."

        wrapMode: Label.WordWrap
        verticalAlignment: Label.AlignTop
    }
}
