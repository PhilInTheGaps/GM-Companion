pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import FileDialog
import ".."

Dialog {
    id: root

    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    width: parent.width * 0.8
    height: parent.height * 0.8

    modal: true

    property alias selectedPath: selection_text_field.text
    property alias folder: backend.currentDir
    property alias foldersOnly: backend.folderMode
    property var textField: undefined
    property string replacePath: ""

    FileDialogBackend {
        id: backend
    }

    onAccepted: {
        if (textField) {
            if (replacePath) {
                textField.text = selection_text_field.text.replace(replacePath, "");
            } else {
                textField.text = selection_text_field.text;
            }
            if (textField.savePath) {
                textField.savePath();
            }
        }
    }

    header: ToolBar {
        Row {
            id: header_row_left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.margins: 5

            CustomToolBarButton {
                iconText: FontAwesome.chevronLeft
                onClicked: backend.back()
                enabled: backend.canGoBack
            }

            CustomToolBarButton {
                iconText: FontAwesome.chevronRight
                onClicked: backend.forward()
                enabled: backend.canGoForward
            }
        }

        TextField {
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

                onClicked: {
                    new_folder_form.visible = !new_folder_form.visible;
                }
            }
        }
    }

    contentItem: Item {
        id: main

        Item {
            id: new_folder_form

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: new_folder_form_row_icon.height + 10
            visible: false

            Label {
                id: new_folder_form_row_icon
                text: FontAwesome.folder
                font.family: FontAwesome.fontSolid.family
                font.styleName: FontAwesome.fontSolid.styleName
                width: height
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
            }

            TextField {
                placeholderText: qsTr("New Folder")
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: new_folder_form_row_icon.right
                anchors.leftMargin: 5

                onEditingFinished: {
                    if (text.length > 0) {
                        backend.createFolder(text);
                    }
                    new_folder_form.visible = false;
                    clear();
                }
            }
        }

        ListView {
            id: main_list_view
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.top: new_folder_form.visible ? new_folder_form.bottom : parent.top
            anchors.topMargin: new_folder_form.visible ? 5 : 0

            clip: true
            highlightMoveVelocity: -1
            interactive: true

            ScrollBar.vertical: ScrollBar {
                id: scroll_bar
                visible: main_list_view.contentHeight > main_list_view.height
            }

            onCurrentIndexChanged: {
                selection_text_field.text = backend.getSelected(currentIndex);
            }

            model: backend.entries

            delegate: Rectangle {
                id: delegate_item

                required property int index
                required property FileObject modelData

                anchors.left: parent ? parent.left : undefined
                anchors.right: parent ? parent.right : undefined
                anchors.leftMargin: 1
                anchors.rightMargin: scroll_bar.visible ? scroll_bar.width : 0
                height: delegate_row.height

                color: "transparent"
                border.color: delegate_mouse_area.pressed ? Colors.buttonPressed : palette.button
                border.width: delegate_mouse_area.containsMouse ? 1 : 0

                Row {
                    id: delegate_row
                    anchors.left: parent.left
                    anchors.right: parent.right
                    padding: 5
                    leftPadding: 10
                    spacing: 10

                    Label {
                        text: delegate_item.modelData.isFolder ? FontAwesome.folder : FontAwesome.file
                        font.family: FontAwesome.fontSolid.family
                        font.styleName: FontAwesome.fontSolid.styleName
                        width: height
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Label {
                        text: delegate_item.modelData.name
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                MouseArea {
                    id: delegate_mouse_area
                    anchors.fill: delegate_item
                    hoverEnabled: true
                    onClicked: main_list_view.currentIndex = delegate_item.index
                    onDoubleClicked: backend.enterFolder(delegate_item.index)
                }
            }

            highlight: Rectangle {
                color: palette.highlight
            }
        }
    }

    footer: Rectangle {
        height: Sizes.toolbarHeight
        color: palette.dark

        TextField {
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

            Button {
                id: open_button
                text: qsTr("Open")

                anchors.top: parent.top
                anchors.bottom: parent.bottom

                width: Math.max(open_button.implicitWidth, cancel_button.implicitWidth)

                onClicked: {
                    root.accept();
                }
            }

            Button {
                id: cancel_button
                text: qsTr("Cancel")

                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: Math.max(open_button.implicitWidth, cancel_button.implicitWidth)

                onClicked: root.close()
            }
        }
    }
}
