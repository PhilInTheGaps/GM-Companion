import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

import FontAwesome 2.0
import "../../buttons"

Rectangle {
    id: file_list_view

    color: color_scheme.backgroundColor

    Connections {
        target: audio_editor
        onFileIndexChanged: file_list.currentIndex = index
        onCurrentElementChanged: console.log("Element changed!")
    }

    FileDialog {
        id: file_dialog
        title: qsTr("Set Folder")
        property int index: 0

        folder: (platform.isWindows ? "file:///" : "file://") + audio_editor.basePath(
                    audio_editor.type)

        selectFolder: true

        onAccepted: {
            audio_editor.replaceFileFolder(audio_editor.name,
                                           audio_editor.type, index,
                                           fileUrl.toString())
        }
    }

    ListView {
        id: file_list
        anchors.fill: parent
        anchors.rightMargin: 5

        model: editorFileModel
        interactive: true
        clip: true
        focus: true
        keyNavigationEnabled: true
        highlightFollowsCurrentItem: false

        property int last_index: 0

        delegate: Item {
            id: delegate_root

            width: parent.width
            height: ListView.isCurrentItem ? color_scheme.toolbarHeight : delegate_label.height + 25

            Rectangle {
                anchors.fill: parent
                color: modelData.missing ? "darkred" : (file_list.currentIndex == index ? color_scheme.menuColor : color_scheme.backgroundColor)
            }

            Text {
                id: delegate_label
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.right: parent.ListView.isCurrentItem ? delegate_row.left : parent.right
                anchors.margins: 5

                text: modelData.title != "" ? modelData.title + "  |  "
                                              + modelData.url : modelData.url
                color: file_list.currentIndex
                       == index ? color_scheme.toolbarTextColor : color_scheme.textColor
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

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
                spacing: 10

                // Folder
                ControlBarButton {
                    visible: modelData.source === 0
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
                            delegate_root.ListView.view.last_index
                                    = delegate_root.ListView.view.currentIndex

                            audio_editor.moveFile(audio_editor.name,
                                                  audio_editor.type, index, -1)

                            if (delegate_root.ListView.view.last_index > 0)
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

                            if (delegate_root.ListView.view.currentIndex
                                    < delegate_root.ListView.view.lastIndex) {
                                delegate_root.ListView.view.currentIndex
                                        = delegate_root.ListView.view.last_index + 1
                            }

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
                        delegate_root.ListView.view.last_index = index
                        audio_editor.removeFile(audio_editor.name,
                                                audio_editor.type, index, false)
                        delegate_root.ListView.view.currentIndex
                                = delegate_root.ListView.view.last_index
                        delegate_root.ListView.view.positionViewAtIndex(
                                    delegate_root.ListView.view.currentIndex,
                                    ListView.Center)
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
}
