pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import src
import "../../../.."
import "../../../../common"

Item {
    id: file_list_view

    CustomFileDialog {
        id: file_dialog

        property int index: 0

        foldersOnly: true
        onAccepted: {
            AudioTool.editor.replaceFileFolder(index, selectedPath);
        }
    }

    ListView {
        id: file_list
        anchors.fill: parent
        anchors.margins: 5
        anchors.rightMargin: 0

        model: AudioTool.editor.files
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

            required property AudioFile modelData
            required property int index

            anchors.left: parent ? parent.left : undefined
            anchors.right: parent ? parent.right : undefined
            anchors.rightMargin: scroll_bar.visible ? 10 : 5

            height: ListView.isCurrentItem ? Sizes.toolbarHeight : delegate_label.height + 25

            // Background
            Rectangle {
                anchors.fill: parent
                color: "transparent"
                border.color: parent.ListView.isCurrentItem ? palette.button : palette.dark
                border.width: parent.ListView.isCurrentItem ? 2 : mouse_area.containsMouse || (delegate_root.modelData && delegate_root.modelData.missing) ? 1 : 0
            }

            // File path label
            Label {
                id: delegate_label
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.right: parent.ListView.isCurrentItem ? delegate_row.left : parent.right
                anchors.margins: 5

                text: delegate_root.modelData ? delegate_root.modelData.printableUrl : ""

                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            // Mouse area for index selection
            MouseArea {
                id: mouse_area
                anchors.fill: parent
                hoverEnabled: true
                onClicked: file_list.currentIndex = delegate_root.index
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
                    visible: delegate_root.modelData && delegate_root.modelData.missing && delegate_root.modelData.source === 0
                    iconText: FontAwesome.folder
                    toolTipText: qsTr("Select folder for file")

                    onClicked: {
                        file_dialog.index = delegate_root.index;
                        file_dialog.folder = AudioTool.editor.basePath();
                        file_dialog.open();
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
                            font.family: FontAwesome.fontSolid.family
                            font.styleName: FontAwesome.fontSolid.styleName
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }

                        onClicked: {
                            delegate_root.ListView.view.lastIndex = delegate_root.ListView.view.currentIndex;
                            AudioTool.editor.moveFile(index, -1);
                            if (delegate_root.ListView.view.lastIndex > 0)
                                delegate_root.ListView.view.currentIndex = delegate_root.ListView.view.lastIndex - 1;
                            delegate_root.ListView.view.positionViewAtIndex(delegate_root.ListView.view.currentIndex, ListView.Center);
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
                            font.family: FontAwesome.fontSolid.family
                            font.styleName: FontAwesome.fontSolid.styleName
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }

                        onClicked: {
                            delegate_root.ListView.view.lastIndex = delegate_root.index;
                            AudioTool.editor.moveFile(index, 1);
                            if (delegate_root.ListView.view.currentIndex < delegate_root.ListView.view.lastIndex) {
                                delegate_root.ListView.view.currentIndex = delegate_root.ListView.view.lastIndex + 1;
                            }
                            delegate_root.ListView.view.positionViewAtIndex(delegate_root.ListView.view.currentIndex, ListView.Center);
                        }
                    }
                }

                // Delete
                CustomToolBarButton {
                    iconText: FontAwesome.trash
                    toolTipText: qsTr("Remove file from list")

                    onClicked: {
                        var modelLength = AudioTool.editor.files.rowCount(); // qmllint disable missing-property
                        var lastIndex = index < (modelLength - 1) ? index : (modelLength - 2);
                        delegate_root.ListView.view.lastIndex = lastIndex;
                        AudioTool.editor.removeFile(index, false);
                        delegate_root.ListView.view.currentIndex = lastIndex;
                        delegate_root.ListView.view.positionViewAtIndex(lastIndex, ListView.Contain);
                    }
                }
            }
        }
    }
}
