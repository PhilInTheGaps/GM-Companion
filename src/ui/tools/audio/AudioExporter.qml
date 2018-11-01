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

        Column {
            id: mid_column

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: left_column.right
            width: (parent.width - left_column.width) / 2

            padding: 10
            spacing: 10

            Text {
                text: qsTr("Categories to Export")
                font.bold: true
                color: colors.textColor
            }

            ScrollView {
                id: cat_scroll
                width: parent.width - parent.padding * 2
                height: parent.height / 2.5
                clip: true

                Column {
                    id: cat_column

                    width: mid_column.width - mid_column.padding * 2
                    spacing: 5

                    Repeater {
                        model: categories

                        Button {
                            id: cat_button
                            width: cat_column.width
                            Row {
                                spacing: 5

                                CheckBox {
                                    id: cat_button_checkbox
                                    checked: tool.isCategoryEnabled(index)

                                    onCheckedChanged: tool.setCategoryEnabled(
                                                          index, checked)
                                }

                                Text {
                                    text: modelData
                                    anchors.verticalCenter: parent.verticalCenter
                                    elide: Text.ElideRight
                                    width: cat_button.width - x
                                }

                                anchors.verticalCenter: parent.verticalCenter
                            }

                            onClicked: {
                                tool.setCategory(index,
                                                 cat_button_checkbox.checked)
                            }
                        }
                    }
                }
            }

            Text {
                text: qsTr("Scenarios to Export")
                font.bold: true
                color: colors.textColor
            }

            ScrollView {
                id: scen_scroll
                width: parent.width - parent.padding * 2
                height: parent.height / 2.5
                clip: true

                Column {
                    id: scen_column

                    width: mid_column.width - mid_column.padding * 2
                    spacing: 5

                    Repeater {
                        model: scenarios

                        Button {
                            id: scen_button
                            width: scen_column.width
                            Row {
                                spacing: 5

                                CheckBox {
                                    id: scen_button_checkbox
                                    checked: tool.isScenarioEnabled(index)

                                    onCheckedChanged: tool.setScenarioEnabled(
                                                          index, checked)
                                }

                                Text {
                                    text: modelData
                                    anchors.verticalCenter: parent.verticalCenter
                                    elide: Text.ElideRight
                                    width: scen_button.width - x
                                }

                                anchors.verticalCenter: parent.verticalCenter
                            }

                            onClicked: {
                                tool.setScenario(index,
                                                 scen_button_checkbox.checked)
                            }
                        }
                    }
                }
            }
        }

        Column {
            id: right_column

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: mid_column.right
            anchors.right: parent.right

            padding: 10
            spacing: 10

            Text {
                text: qsTr("Elements to Export")
                font.bold: true
                color: colors.textColor
            }

            ScrollView {
                id: element_scroll
                width: parent.width - parent.padding * 2
                height: parent.height - x - parent.bottomPadding
                clip: true

                Column {
                    id: element_column

                    width: right_column.width - right_column.padding * 2
                    spacing: 5

                    Repeater {
                        model: elements

                        Rectangle {
                            id: element_button
                            width: element_column.width
                            height: element_button_row.height

                            Row {
                                id: element_button_row
                                spacing: 5

                                CheckBox {
                                    id: element_button_checkbox
                                    checked: tool.isElementEnabled(index)

                                    onCheckedChanged: tool.setElementEnabled(
                                                          index, checked)
                                }

                                Text {
                                    text: modelData
                                    anchors.verticalCenter: parent.verticalCenter
                                    elide: Text.ElideRight
                                    width: element_button.width - x
                                }

                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                    }
                }
            }
        }
    }

    background: Rectangle {
        color: colors.backgroundColor
    }

    standardButtons: Dialog.Ok | Dialog.Cancel

    onAccepted: {
        tool.exportFiles()
    }
}
