import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.3 as OldDialogs

Dialog {
    id: dialog
    title: qsTr("Export Audio Files")
    modal: true

    OldDialogs.FileDialog {
        id: file_dialog

        property var text_field

        selectFolder: true

        onAccepted: {
            if (platform.isWindows)
                path_text_field.text = fileUrl.toString().replace("file:///",
                                                                  "")
            else
                path_text_field.text = fileUrl.toString().replace("file://", "")
        }
    }

    contentItem: Page {
        background: Rectangle {
            color: "transparent"
        }

        Column {
            id: left_column

            width: parent.width / 4
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.bottom: parent.bottom

            spacing: 10
            padding: 10

            Text {
                text: qsTr("Settings")
                color: color_scheme.textColor
            }

            TextField {
                id: path_text_field

                width: parent.width - parent.padding * 2
                selectByMouse: true
                text: audio_exporter.getDefaultPath()

                onTextChanged: audio_exporter.setPath(text)
            }

            Button {
                text: qsTr("Set Folder")

                onClicked: {
                    file_dialog.setFolder("file://" + path_text_field.text)
                    file_dialog.open()
                }
            }
        }

        CheckScrollView {
            id: cat_scroll

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: left_column.right
            width: (parent.width - left_column.width) / 3

            headline: qsTr("Categories to Export")
            repModel: audio_exporter.categories
            checkedFunc: audio_exporter.isCategoryEnabled

            onItemChecked: audio_exporter.setCategoryEnabled(index, checked)
            onItemClicked: audio_exporter.setCategory(index)
        }

        CheckScrollView {
            id: scen_scroll

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: cat_scroll.right
            width: (parent.width - left_column.width) / 3

            headline: qsTr("Scenarios to Export")
            repModel: audio_exporter.scenarios
            checkedFunc: audio_exporter.isScenarioEnabled

            onItemChecked: audio_exporter.setScenarioEnabled(index, checked)
            onItemClicked: audio_exporter.setScenario(index)
        }

        CheckScrollView {
            id: ele_scroll

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: scen_scroll.right
            anchors.right: parent.right

            headline: qsTr("Elements to Export")
            repModel: audio_exporter.elements
            checkedFunc: audio_exporter.isElementEnabled

            onItemChecked: audio_exporter.setElementEnabled(index, checked)
        }
    }

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    footer: Rectangle {
        height: buttons.height
        color: color_scheme.backgroundColor

        ProgressBar {
            id: progress_bar
            value: audio_exporter.progress
            anchors.left: parent.left
            anchors.right: buttons.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 15
        }

        DialogButtonBox {
            id: buttons
            anchors.right: parent.right
            anchors.top: parent.top

            Button {
                text: qsTr("Export")
                DialogButtonBox.buttonRole: DialogButtonBox.ApplyRole
            }

            standardButtons: Dialog.Close

            onApplied: {
                audio_exporter.exportFiles()
            }

            onRejected: {
                dialog.close()
            }
        }
    }
}
