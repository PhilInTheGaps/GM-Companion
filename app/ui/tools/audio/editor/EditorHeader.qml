import QtQuick 2.6
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0

CustomToolBar {
    id: tool_bar

    enableBack: true
    enableAdd: true
    enableSave: true
    enableExport: true

    isSaved: audio_editor.isSaved

    onBackClicked: backToTool()
    onAddClicked: new_thing_dialog.open()
    onSaveClicked: audio_editor.saveProject()
    onExportClicked: {
        audio_exporter.project = project_box.currentText
        audio_exporter_dialog.open()
    }

    Row {
        anchors.left: tool_bar.button_row.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        padding: 10
        spacing: 5

        CustomToolBarComboBox {
            id: project_box
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 150

            model: audio_editor.projectNames
            currentIndex: audio_editor.projectIndex

            onCurrentIndexChanged: audio_editor.setCurrentProject(currentIndex)
        }

        CustomToolBarButton {
            anchors.margins: 10
            iconText: FontAwesome.pen
            onClicked: {
                rename_dialog.mode = 0
                rename_dialog.title = qsTr("Rename Project")
                rename_dialog.origName = project_box.currentText
                rename_dialog.x = project_box.x
                rename_dialog.y = tool_bar.height
                rename_dialog.open()
            }
        }

        CustomToolBarButton {
            anchors.margins: 10
            iconText: FontAwesome.trash
            onClicked: {
                delete_dialog.x = project_box.x
                delete_dialog.y = tool_bar.height
                delete_dialog.element_name = project_box.currentText
                delete_dialog.mode = 0
                delete_dialog.open()
            }
        }

        CustomToolBarComboBox {
            id: category_box
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 150

            model: audio_editor.categoryNames
            currentIndex: audio_editor.categoryIndex

            onCurrentTextChanged: audio_editor.setCurrentCategory(currentText)
        }

        CustomToolBarButton {
            anchors.margins: 10
            iconText: FontAwesome.pen
            onClicked: {
                rename_dialog.mode = 1
                rename_dialog.title = qsTr("Rename Category")
                rename_dialog.origName = category_box.currentText
                rename_dialog.x = category_box.x
                rename_dialog.y = tool_bar.height
                rename_dialog.open()
            }
        }

        CustomToolBarButton {
            anchors.margins: 10
            iconText: FontAwesome.trash
            onClicked: {
                delete_dialog.x = category_box.x
                delete_dialog.y = tool_bar.height
                delete_dialog.element_name = category_box.currentText
                delete_dialog.mode = 1
                delete_dialog.open()
            }
        }

        CustomToolBarComboBox {
            id: scenario_box
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 150

            model: audio_editor.scenarioNames
            currentIndex: audio_editor.scenarioIndex

            onCurrentTextChanged: {
                audio_editor.setCurrentScenario(currentText)
            }
        }

        CustomToolBarButton {
            anchors.margins: 10
            iconText: FontAwesome.pen
            onClicked: {
                rename_dialog.mode = 2
                rename_dialog.title = qsTr("Rename Scenario")
                rename_dialog.origName = scenario_box.currentText
                rename_dialog.x = scenario_box.x
                rename_dialog.y = tool_bar.height
                rename_dialog.open()
            }
        }

        CustomToolBarButton {
            anchors.margins: 10
            iconText: FontAwesome.trash
            onClicked: {
                delete_dialog.x = scenario_box.x
                delete_dialog.y = tool_bar.height
                delete_dialog.element_name = scenario_box.currentText
                delete_dialog.mode = 2
                delete_dialog.open()
            }
        }

        // Spacer
        Item {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 10
        }

        // Sort Elements
        CustomToolBarButton {
            iconText: FontAwesome.sortAlphaDown
            onClicked: audio_editor.sortElements()
        }

        // View Mode
        CustomToolBarButton {
            iconText: element_column.small_mode ? FontAwesome.expand : FontAwesome.compress
            onClicked: element_column.small_mode = !element_column.small_mode
        }

        CustomToolBarButton {
            iconText: FontAwesome.fileAudio
            iconColor: "darkred"

            ToolTip.text: qsTr("Remove missing files.")
            ToolTip.visible: hovered

            Label {
                text: FontAwesome.trashAlt
                font.family: FontAwesome.familySolid
                font.pixelSize: height

                height: parent.height / 3
                width: height
                x: parent.width - width * 1.5
                y: parent.height - height * 1.5

                color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"

                background: Rectangle {
                    color: palette.alternateBase
                }
            }

            onClicked: audio_editor.removeMissingFiles(audio_editor.name,
                                                       audio_editor.type)
        }
    }
}
