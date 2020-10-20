import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0

import "../../buttons"
import "../../../../sizes.js" as Sizes
import "../../../../colors.js" as Colors
import "../../../../common"

Item {
    id: file_list_view

    FileDialog {
        id: file_dialog

        property int index: 0

        foldersOnly: true
        onAccepted: {
            audio_editor.replaceFileFolder(index, selectedPath)
        }
    }

    ListView {
        id: file_list
        anchors.fill: parent
        anchors.margins: 5
        anchors.rightMargin: 0

        model: audio_editor_file_model
        interactive: true
        clip: true
        focus: true
        keyNavigationEnabled: true
        highlightFollowsCurrentItem: false

        property int lastIndex: 0

        ScrollBar.vertical: ScrollBar {
            id: scroll_bar
            visible: file_list.contentHeight > file_list.height
        }

        delegate: Item {
            id: delegate_root

            anchors.left: parent ? parent.left : undefined
            anchors.right: parent ? parent.right : undefined
            anchors.rightMargin: scroll_bar.visible ? 10 : 5

            height: ListView.isCurrentItem ? Sizes.toolbarHeight : delegate_label.height + 25

            // Background
            Rectangle {
                anchors.fill: parent
                color: modelData
                       && modelData.missing ? "darkred" : Colors.window
                border.color: parent.ListView.isCurrentItem ? Colors.alternateBase : Colors.dark
                border.width: parent.ListView.isCurrentItem ? 2 : mouse_area.containsMouse ? 1 : 0
            }

            // File path label
            Label {
                id: delegate_label
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.right: parent.ListView.isCurrentItem ? delegate_row.left : parent.right
                anchors.margins: 5

                text: modelData ? modelData.printableUrl : ""

                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            // Mouse area for index selection
            MouseArea {
                id: mouse_area
                anchors.fill: parent
                hoverEnabled: true
                onClicked: file_list.currentIndex = index
            }

            // Buttons on the right
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
                    visible: modelData && modelData.missing
                             && modelData.source === 0
                    iconText: FontAwesome.folder
                    toolTipText: qsTr("Select folder for file")

                    onClicked: {
                        file_dialog.index = index
                        file_dialog.folder = audio_editor.basePath()
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
                            delegate_root.ListView.view.lastIndex
                                    = delegate_root.ListView.view.currentIndex

                            audio_editor.moveFile(index, -1)

                            if (delegate_root.ListView.view.lastIndex > 0)
                                delegate_root.ListView.view.currentIndex
                                        = delegate_root.ListView.view.lastIndex - 1

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
                            delegate_root.ListView.view.lastIndex = index
                            audio_editor.moveFile(index, 1)

                            if (delegate_root.ListView.view.currentIndex
                                    < delegate_root.ListView.view.lastIndex) {
                                delegate_root.ListView.view.currentIndex
                                        = delegate_root.ListView.view.lastIndex + 1
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
                    toolTipText: qsTr("Remove file from list")

                    onClicked: {
                        var modelLength = audio_editor_file_model.rowCount()
                        var lastIndex = index < (modelLength - 1) ? index : (modelLength - 2)

                        console.log(index, lastIndex, modelLength)

                        delegate_root.ListView.view.lastIndex = lastIndex
                        audio_editor.removeFile(index, false)
                        delegate_root.ListView.view.currentIndex = lastIndex
                        delegate_root.ListView.view.positionViewAtIndex(
                                    lastIndex, ListView.Contain)
                    }
                }
            }
        }
    }
}
