import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4

import gm.companion.audioeditorfilebrowser 1.0
import gm.companion.colorscheme 1.0
import "../../fontawesome"

Column {
    id: file_browser

    signal addFile(string file, string path)
    signal addAllFiles(var fileNames, var filePaths)

    property int fileType

    AudioEditorFileBrowserTool {
        id: tool

        function folderButtonClicked(folder, path) {
            setCurrentFolder(folder)
        }

        onFoldersChanged: {
            folder_column.children = []

            for (var i = 0; i < getFolderList().length; i++) {
                var component = Qt.createComponent("./FileBrowserButton.qml")

                var button = component.createObject(folder_column, {
                                                        x: 0,
                                                        y: 0,
                                                        element: getFolderList(
                                                                     )[i],
                                                        path: getFolderPaths(
                                                                  )[i],
                                                        type: 3
                                                    })
                button.clicked.connect(folderButtonClicked)
            }
        }

        function fileButtonClicked(file, path) {
            addFile(file, path)
        }

        onFilesChanged: {
            file_column.children = []

            for (var i = 0; i < getFileList().length; i++) {
                var component = Qt.createComponent("./FileBrowserButton.qml")

                var button = component.createObject(file_column, {
                                                        x: 0,
                                                        y: 0,
                                                        element: getFileList(
                                                                     )[i],
                                                        path: getFilePaths()[i],
                                                        type: tool.getType()
                                                    })
                button.clicked.connect(fileButtonClicked)
            }
        }
    }

    function setType(type) {
        tool.setType(type)
        fileType = type
    }

    padding: 5
    spacing: 5

    ScrollView {
        id: scroll_view
        width: parent.width - parent.padding * 2
        height: parent.height - parent.padding * 2
        clip: true

        Column {
            id: scroll_view_column
            width: file_browser.width - file_browser.padding * 2
            spacing: 5

            Text {
                text: qsTr("Folders:")
                color: color_scheme.textColor
            }

            Button {
                id: folder_back_button
                width: parent.width
                text: qsTr("Back")

                Icon {
                    icon: icons.fas_chevron_left
                    pointSize: 15
                    anchors.verticalCenter: parent.verticalCenter

                    x: 10
                }

                onClicked: tool.folderBack()
            }

            Column {
                id: folder_column
                width: parent.width
                spacing: 5
            }

            Rectangle {
                width: parent.width
                height: 10
                color: "transparent"
            }

            Text {
                text: qsTr("Files:")
                color: color_scheme.textColor
            }

            Button {
                width: parent.width
                text: qsTr("Add All Files")
                visible: file_browser.fileType !== 2

                onClicked: addAllFiles(tool.getFileList(), tool.getFilePaths())
            }

            Column {
                id: file_column
                width: parent.width
                spacing: 5
            }
        }
    }
}
