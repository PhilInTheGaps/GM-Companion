import QtQuick
import QtQuick.Controls
import IconFonts
import src
import "../../../common"
import "../../.."

Dialog {
    id: dialog
    title: qsTr("Export Audio Files")
    modal: true

    CustomFileDialog {
        id: file_dialog
        foldersOnly: true
        textField: path_text_field
    }

    contentItem: Item {
        Rectangle {
            id: left_item
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            width: parent.width / 4

            color: palette.dark
            border.color: Colors.border
            border.width: 1

            TreeView {
                id: tree_view

                anchors.fill: parent
                anchors.margins: 5

                model: AudioTool.editor.exporter.model ? AudioTool.editor.exporter.model.childItems : [] // qmllint disable missing-property
                itemIcon: FontAwesome.bars
            }
        }

        Item {
            id: right_item
            anchors.top: parent.top
            anchors.left: left_item.right
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.leftMargin: 10

            Column {
                id: settings_column

                anchors.fill: parent
                spacing: 10

                Label {
                    text: qsTr("Settings")
                }

                TextField {
                    id: path_text_field

                    enabled: !progress_bar.inProgress

                    width: parent.width - parent.padding * 2
                    selectByMouse: true
                    placeholderText: qsTr("Export Folder")

                    onTextChanged: AudioTool.editor.exporter.setPath(text)
                }

                Button {
                    text: qsTr("Set Folder")

                    enabled: !progress_bar.inProgress

                    onClicked: {
                        file_dialog.folder = path_text_field.text
                        file_dialog.open()
                    }
                }
            }

            Item {
                height: buttons.height
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                ProgressBar {
                    id: progress_bar

                    property bool inProgress: Math.abs(
                                                  progress_bar.value - 0) >= Number.EPSILON

                    value: AudioTool.editor.exporter.progress
                    anchors.left: parent.left
                    anchors.right: buttons.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: 5
                }

                Row {
                    anchors.right: progress_bar.right
                    anchors.verticalCenter: progress_bar.verticalCenter
                    padding: 5
                    spacing: 5

                    Label {
                        anchors.verticalCenter: parent.verticalCenter
                        text: Math.round(progress_bar.value * 100)
                    }

                    Label {
                        anchors.verticalCenter: parent.verticalCenter
                        text: "%"
                    }
                }

                DialogButtonBox {
                    id: buttons
                    anchors.right: parent.right
                    anchors.top: parent.top
                    padding: 0

                    Button {
                        text: qsTr("Export")
                        enabled: !progress_bar.inProgress

                        DialogButtonBox.buttonRole: DialogButtonBox.ApplyRole
                        onClicked: AudioTool.editor.exporter.exportFiles()
                    }

                    standardButtons: Dialog.Close

                    onRejected: {
                        dialog.close()
                    }
                }
            }
        }
    }
}
