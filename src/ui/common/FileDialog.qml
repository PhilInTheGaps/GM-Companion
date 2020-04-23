import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import lol.rophil.gmcompanion.filedialog 1.0
import "../defines.js" as Defines

Dialog {
    id: root

    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    width: parent.width * 0.8
    height: parent.height * 0.8

    modal: true

    property alias folder: backend.currentDir
    property alias foldersOnly: backend.folderMode
    property var textField: undefined

    FileDialogBackend {
        id: backend
    }

    onOpened: {
        backend.clearForward()
    }

    onAccepted: {
        textField.text = selection_text_field.text
        textField.savePath()
    }

    header: Rectangle {
        height: Defines.TOOLBAR_HEIGHT
        color: palette.alternateBase

        Row {
            id: header_row_left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.margins: 5

            CustomToolBarButton {
                iconText: FontAwesome.chevronLeft
                onClicked: backend.back()
            }

            CustomToolBarButton {
                iconText: FontAwesome.chevronRight
                onClicked: backend.forward()
                enabled: backend.canForward
            }
        }

        CustomTextField {
            id: header_text_field
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: header_row_left.right
            anchors.right: header_row_right.left
            anchors.margins: 5
            text: backend.currentDir
            onTextEdited: backend.currentDir = text
        }

        Row {
            id: header_row_right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.margins: 5

            CustomToolBarButton {
                iconText: FontAwesome.folderPlus
            }
        }
    }

    contentItem: Item {
        id: main

        ListView {
            id: main_list_view
            anchors.fill: parent

            clip: true
            highlightMoveVelocity: -1
            interactive: true

            ScrollBar.vertical: CustomScrollBar {
                id: scroll_bar
                visible: main_list_view.contentHeight > main_list_view.height
            }

            onCurrentIndexChanged: {
                selection_text_field.text = backend.getSelected(currentIndex)
            }

            model: backend.files

            delegate: Item {
                id: delegate_item
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: scroll_bar.visible ? scroll_bar.width : 0
                height: delegate_row.height

                Row {
                    id: delegate_row
                    anchors.left: parent.left
                    anchors.right: parent.right
                    padding: 5
                    spacing: 10

                    Label {
                        text: modelData.isFolder ? FontAwesome.folder : FontAwesome.file
                        font.family: FontAwesome.familySolid
                        width: height
                        verticalAlignment: Text.AlignVCenter
                    }

                    Label {
                        text: modelData.name
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                MouseArea {
                    id: delegate_mouse_area
                    anchors.fill: delegate_item
                    hoverEnabled: true
                    onClicked: main_list_view.currentIndex = index
                    onDoubleClicked: backend.enterFolder(index)
                }
            }

            highlight: Rectangle {
                color: palette.highlight
            }
        }
    }

    footer: Rectangle {
        height: Defines.TOOLBAR_HEIGHT
        color: palette.dark

        CustomTextField {
            id: selection_text_field
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: buttons.left
            anchors.margins: 5
        }

        Row {
            id: buttons
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 5
            spacing: 5

            CustomButton {
                id: open_button
                buttonText: qsTr("Open")
                usesFixedWidth: false
                anchors.top: parent.top
                anchors.bottom: parent.bottom

                textItem.horizontalAlignment: Text.AlignHCenter
                width: Math.max(open_button.implicitWidth,
                                cancel_button.implicitWidth)

                onClicked: {
                    root.accept()
                }
            }

            CustomButton {
                id: cancel_button
                buttonText: qsTr("Cancel")
                usesFixedWidth: false
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                textItem.horizontalAlignment: Text.AlignHCenter
                width: Math.max(open_button.implicitWidth,
                                cancel_button.implicitWidth)

                onClicked: root.close()
            }
        }
    }
}
