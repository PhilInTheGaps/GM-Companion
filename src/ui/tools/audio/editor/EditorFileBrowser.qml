import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import FontAwesome 2.0
import "../buttons"

Item {
    id: file_browser

    Connections {
        target: audio_editor_file_browser
    }

    Connections {
        target: audio_editor

        onCurrentElementChanged: {
            audio_editor_file_browser.type = audio_editor.type
        }
    }

    Rectangle {
        id: browser_control_bar

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: color_scheme.toolbarHeight
        color: color_scheme.menuColor

        Row {
            anchors.fill: parent
            spacing: 5
            leftPadding: 5
            rightPadding: 5

            ControlBarButton {
                fa_icon: FontAwesome.chevronUp
                onClicked: audio_editor_file_browser.folderBack()
            }

            ControlBarButton {
                fa_icon: FontAwesome.home
                onClicked: audio_editor_file_browser.home()
            }

            ControlBarButton {
                visible: audio_editor.type != 2
                fa_icon: FontAwesome.plus
                onClicked: {
                    audio_editor.setFileIndex(file_list.currentIndex)
                    audio_editor_file_browser.addAllFiles()
                }
            }
        }
    }

    ScrollView {
        id: scroll_view
        anchors.top: browser_control_bar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        clip: true

        Column {
            id: scroll_view_column
            anchors.left: parent.left
            anchors.right: parent.right

            topPadding: 5
            spacing: 5

            // Folder
            Repeater {
                model: audio_editor_file_browser.folderNames

                FileBrowserButton {
                    file: modelData
                    type: 3
                }
            }

            // Files
            Repeater {
                model: audio_editor_file_browser.fileNames

                FileBrowserButton {
                    file: modelData
                    type: audio_editor.type
                    path: audio_editor_file_browser.filePaths[index]
                }
            }
        }
    }
}
