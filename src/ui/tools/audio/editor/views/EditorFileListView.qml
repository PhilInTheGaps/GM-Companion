import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
import CustomComponents 1.0
import FontAwesome 2.0

import "../../buttons"
import "../../../../defines.js" as Defines

Item {
    id: file_list_view

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
            height: ListView.isCurrentItem ? Defines.TOOLBAR_HEIGHT : delegate_label.height + 25

            Rectangle {
                anchors.fill: parent
                color: modelData
                       && modelData.missing ? "darkred" : (file_list.currentIndex == index ? palette.alternateBase : palette.window)
            }

            Label {
                id: delegate_label
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.right: parent.ListView.isCurrentItem ? delegate_row.left : parent.right
                anchors.margins: 5

                text: modelData
                      && modelData.title !== "" ? modelData.title + "  |  "
                                                  + modelData.url : modelData.url

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
                padding: 10

                // Folder
                CustomToolBarButton {
                    visible: modelData && modelData.source === 0
                    iconText: FontAwesome.folder

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

                        Label {
                            text: FontAwesome.chevronUp
                            font.family: FontAwesome.familySolid
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

                        Label {
                            text: FontAwesome.chevronDown
                            font.family: FontAwesome.familySolid
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
                CustomToolBarButton {
                    iconText: FontAwesome.trashAlt

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

        ScrollBar.vertical: ScrollBar {}
    }
}
