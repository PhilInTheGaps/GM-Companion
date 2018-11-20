import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.3 as OldDialogs

import gm.companion.audioexporter 1.0
import gm.companion.colorscheme 1.0
import gm.companion.platforms 1.0

Dialog {
    id: dialog
    title: qsTr("Export Audio Files")
    modal: true

    property alias project: tool.project
    property alias categories: tool.categories
    property alias scenarios: tool.scenarios
    property alias elements: tool.elements

    AudioExporter {
        id: tool

        onProgressChanged: {
            progress_bar.value = progress
        }
    }

    ColorScheme {
        id: colors
    }

    PlatformDetails {
        id: platform_details
    }

    OldDialogs.FileDialog {
        id: file_dialog

        property var text_field

        selectFolder: true

        onAccepted: {
            if (platform_details.isWindows)
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
                color: colors.textColor
            }

            TextField {
                id: path_text_field
                width: parent.width - parent.padding * 2
                selectByMouse: true
                text: tool.getDefaultPath()

                onTextChanged: tool.setPath(text)
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
            repModel: categories
            checkedFunc: tool.isCategoryEnabled

            onItemChecked: tool.setCategoryEnabled(index, checked)
            onItemClicked: tool.setCategory(index, checked)
        }

        CheckScrollView {
            id: scen_scroll

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: cat_scroll.right
            width: (parent.width - left_column.width) / 3

            headline: qsTr("Scenarios to Export")
            repModel: scenarios
            checkedFunc: tool.isScenarioEnabled

            onItemChecked: tool.setScenarioEnabled(index, checked)
            onItemClicked: tool.setScenario(index, checked)
        }

        CheckScrollView {
            id: ele_scroll1

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: scen_scroll.right
            anchors.right: parent.right

            headline: qsTr("Elements to Export")
            repModel: elements
            checkedFunc: tool.isElementEnabled

            onItemChecked: tool.setElementEnabled(index, checked)
        }
    }

    background: Rectangle {
        color: colors.backgroundColor
    }

    footer: Rectangle {
        height: buttons.height
        color: color_scheme.backgroundColor

        ProgressBar {
            id: progress_bar
            value: 0
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
                tool.exportFiles()
            }

            onRejected: {
                dialog.close()
            }
        }
    }
}
