import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

import FontAwesome 2.0
import "../buttons"

Page {
    id: audio_list_page

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    Connections {
        target: audio_editor
        onFileIndexChanged: file_list.currentIndex = index
    }

    FileDialog {
        id: file_dialog
        title: qsTr("Set Folder")
        property int index: 0

        folder: "file://" + audio_editor.basePath(audio_editor.type)

        selectFolder: true

        onAccepted: {
            audio_editor.replaceFileFolder(audio_editor.name,
                                           audio_editor.type, index,
                                           fileUrl.toString())
        }
    }

    ListView {
        id: file_list
        anchors.left: parent.left
        anchors.right: file_browser.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.rightMargin: 5

        model: audio_editor.files
        interactive: true
        clip: true
        focus: true
        keyNavigationEnabled: true
        highlightFollowsCurrentItem: false

        delegate: Item {
            id: delegate_root

            Rectangle {
                anchors.fill: parent

                color: audio_editor.missing[index] ? "darkred" : file_list.currentIndex
                                                     == index ? color_scheme.menuColor : "white"
            }

            Text {
                id: delegate_label
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.right: parent.ListView.isCurrentItem ? delegate_row.left : parent.right
                anchors.margins: 5

                text: modelData
                color: file_list.currentIndex
                       == index ? color_scheme.toolbarTextColor : color_scheme.textColor
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            width: parent.width
            height: ListView.isCurrentItem ? color_scheme.toolbarHeight : delegate_label.height + 10

            MouseArea {
                id: mouse_area
                anchors.fill: parent
                onClicked: file_list.currentIndex = index
            }

            Row {
                id: delegate_row

                visible: parent.ListView.isCurrentItem

                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                spacing: 5

                // Folder
                ControlBarButton {
                    fa_icon: FontAwesome.folder

                    onClicked: {
                        file_dialog.index = index
                        file_dialog.open()
                    }
                }

                Item {
                    id: file_up_down
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.margins: 5
                    width: height / 2

                    Button {
                        id: file_up
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: parent.height / 2

                        background: Rectangle {
                            color: "transparent"
                        }

                        Text {
                            text: FontAwesome.chevronUp
                            font.family: FontAwesome.familySolid
                            color: color_scheme.toolbarTextColor
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }

                        onClicked: {
                            delegate_root.ListView.view.last_index = index
                            audio_editor.moveFile(audio_editor.name,
                                                  audio_editor.type, index, -1)
                            delegate_root.ListView.view.currentIndex
                                    = delegate_root.ListView.view.last_index - 1
                            delegate_root.ListView.view.positionViewAtIndex(
                                        delegate_root.ListView.view.currentIndex,
                                        ListView.Center)
                        }
                    }

                    Button {
                        id: file_down
                        anchors.top: file_up.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom

                        background: Rectangle {
                            color: "transparent"
                        }

                        Text {
                            text: FontAwesome.chevronDown
                            font.family: FontAwesome.familySolid
                            color: color_scheme.toolbarTextColor
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }

                        onClicked: {
                            delegate_root.ListView.view.last_index = index
                            audio_editor.moveFile(audio_editor.name,
                                                  audio_editor.type, index, 1)
                            delegate_root.ListView.view.currentIndex
                                    = delegate_root.ListView.view.last_index + 1
                            delegate_root.ListView.view.positionViewAtIndex(
                                        delegate_root.ListView.view.currentIndex,
                                        ListView.Center)
                        }
                    }
                }

                // Delete
                ControlBarButton {
                    fa_icon: FontAwesome.trashAlt

                    onClicked: {
                        audio_editor.removeFile(audio_editor.name,
                                                audio_editor.type, index)
                    }
                }
            }
        }

        highlight: Rectangle {
            color: "lightsteelblue"
        }

        ScrollBar.vertical: ScrollBar {
        }
    }

    EditorFileBrowser {
        id: file_browser
        width: 175

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: right_item.left
        anchors.rightMargin: 5
    }

    Rectangle {
        id: right_item

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: color_scheme.toolbarHeight

        color: color_scheme.menuColor

        Column {
            anchors.fill: parent

            Button {
                anchors.left: parent.left
                anchors.right: parent.right
                height: width
                hoverEnabled: true

                background: Rectangle {
                    color: "transparent"
                }

                ToolTip.text: qsTr(
                                  "Playlist is shuffled before playing. Loops.")
                ToolTip.visible: hovered

                Text {
                    text: FontAwesome.listOl
                    font.family: FontAwesome.familySolid
                    font.pixelSize: height
                    anchors.fill: parent
                    anchors.margins: 10
                    color: audio_editor.mode == 0 ? "green" : (parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "lightgrey")
                }

                Label {
                    text: FontAwesome.random
                    font.family: FontAwesome.familySolid
                    font.pixelSize: height

                    height: parent.height / 3
                    width: height
                    x: parent.width - width * 1.5
                    y: parent.height - height * 1.5

                    color: audio_editor.mode == 0 ? "limegreen" : (parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white")

                    background: Rectangle {
                        color: color_scheme.menuColor
                    }
                }

                onClicked: audio_editor.setMode(audio_editor.name,
                                                audio_editor.type, 0)
            }

            Button {
                anchors.left: parent.left
                anchors.right: parent.right
                height: width
                hoverEnabled: true

                background: Rectangle {
                    color: "transparent"
                }

                ToolTip.text: qsTr("Files are played randomly, does not stop.")
                ToolTip.visible: hovered

                Text {
                    text: FontAwesome.random
                    font.family: FontAwesome.familySolid
                    font.pixelSize: height
                    anchors.fill: parent
                    anchors.margins: 10
                    color: audio_editor.mode == 1 ? "limegreen" : (parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white")
                }

                onClicked: audio_editor.setMode(audio_editor.name,
                                                audio_editor.type, 1)
            }

            Button {
                anchors.left: parent.left
                anchors.right: parent.right
                height: width
                hoverEnabled: true

                background: Rectangle {
                    color: "transparent"
                }

                ToolTip.text: qsTr("Playlist loops in sequential order.")
                ToolTip.visible: hovered

                Text {
                    text: FontAwesome.listOl
                    font.family: FontAwesome.familySolid
                    font.pixelSize: height
                    anchors.fill: parent
                    anchors.margins: 10
                    color: audio_editor.mode == 2 ? "green" : (parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "lightgrey")
                }

                Label {
                    text: FontAwesome.sync
                    font.family: FontAwesome.familySolid
                    font.pixelSize: height

                    height: parent.height / 3
                    width: height
                    x: parent.width - width * 1.5
                    y: parent.height - height * 1.5

                    color: audio_editor.mode == 2 ? "limegreen" : (parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white")

                    background: Rectangle {
                        color: color_scheme.menuColor
                    }
                }

                onClicked: audio_editor.setMode(audio_editor.name,
                                                audio_editor.type, 2)
            }

            Button {
                anchors.left: parent.left
                anchors.right: parent.right
                height: width
                hoverEnabled: true

                background: Rectangle {
                    color: "transparent"
                }

                ToolTip.text: qsTr("Playlist is played in set order. Does not loop.")
                ToolTip.visible: hovered

                Text {
                    text: FontAwesome.listOl
                    font.family: FontAwesome.familySolid
                    font.pixelSize: height
                    anchors.fill: parent
                    anchors.margins: 10
                    color: audio_editor.mode == 3 ? "limegreen" : (parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white")
                }

                onClicked: audio_editor.setMode(audio_editor.name,
                                                audio_editor.type, 3)
            }

            Rectangle {
                height: 2
                anchors.left: parent.left
                anchors.right: parent.right
                color: color_scheme.toolbarTextColor
            }

            Button {
                anchors.left: parent.left
                anchors.right: parent.right
                height: width
                hoverEnabled: true

                background: Rectangle {
                    color: "transparent"
                }

                ToolTip.text: qsTr("Remove missing files.")
                ToolTip.visible: hovered

                Text {
                    text: FontAwesome.fileAudio
                    font.family: FontAwesome.familySolid
                    font.pixelSize: height
                    anchors.fill: parent
                    anchors.margins: 10
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "darkred"
                }

                Label {
                    text: FontAwesome.trashAlt
                    font.family: FontAwesome.familySolid
                    font.pixelSize: height

                    height: parent.height / 3
                    width: height
                    x: parent.width - width * 1.5
                    y: parent.height - height * 1.5

                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"

                    background: Rectangle {
                        color: color_scheme.menuColor
                    }
                }

                onClicked: audio_editor.removeMissingFiles(audio_editor.name,
                                                           audio_editor.type)
            }
        }
    }
}
