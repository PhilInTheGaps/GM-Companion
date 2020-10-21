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

    isSaved: audio_editor ? audio_editor.isSaved : true

    onBackClicked: {
        stack.pop()
        loader.active = false
    }

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

            property int projectIndex: audio_editor ? audio_editor.projectIndex : -1

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 150
            textRole: "name"
            emptyString: audio_editor
                         && audio_editor.isLoading ? qsTr("Loading ...") : qsTr(
                                                         "No Projects")

            model: audio_editor ? audio_editor.projects : []

            onCurrentTextChanged: {
                if (audio_editor) {
                    audio_editor.setCurrentProject(currentIndex)
                }
            }

            // If a project is created and set as the current project,
            // change the current index accordingly.
            // Previously this was done by listening to the currentProjectChanged
            // signal of the audio_editor, but the syntax for Connections changed
            // and the old one is deprecated, but we want to still support Qt 5.9
            onProjectIndexChanged: {
                var project = audio_editor.currentProject
                if (project && project.name !== project_box.currentText) {
                    project_box.currentIndex = audio_editor.projectIndex
                }
            }
        }

        CustomToolBarButton {
            anchors.margins: 10
            iconText: FontAwesome.pen
            toolTipText: qsTr("Rename Project")
            enabled: project_box.model.length > 0
            onClicked: {
                rename_dialog.mode = 0
                rename_dialog.title = qsTr("Rename Project")
                rename_dialog.origName = project_box.currentText
                rename_dialog.x = project_box.x
                rename_dialog.y = 0
                rename_dialog.open()
            }
        }

        CustomToolBarButton {
            anchors.margins: 10
            iconText: FontAwesome.trash
            toolTipText: qsTr("Delete Project")
            enabled: project_box.model.length > 0
            onClicked: {
                delete_dialog.x = project_box.x
                delete_dialog.y = 0
                delete_dialog.element = audio_editor.currentProject
                delete_dialog.mode = 0
                delete_dialog.open()
            }
        }

        CustomToolBarComboBox {
            id: category_box
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 150
            textRole: "name"
            emptyString: qsTr("No Categories")

            model: {
                audio_editor
                        && audio_editor.currentProject ? audio_editor.currentProject.categories : []
            }

            currentIndex: {
                audio_editor
                        && audio_editor.currentProject ? audio_editor.currentProject.categoryIndex : -1
            }

            onCurrentTextChanged: {
                if (audio_editor && audio_editor.currentProject
                        && audio_editor.currentProject.currentCategory
                        && audio_editor.currentProject.currentCategory.name
                        !== category_box.currentText) {
                    audio_editor.setCurrentCategory(currentIndex)
                }
            }
        }

        CustomToolBarButton {
            anchors.margins: 10
            iconText: FontAwesome.pen
            toolTipText: qsTr("Rename Category")
            enabled: category_box.model.length > 0
            onClicked: {
                rename_dialog.mode = 1
                rename_dialog.title = qsTr("Rename Category")
                rename_dialog.origName = category_box.currentText
                rename_dialog.x = category_box.x
                rename_dialog.y = 0
                rename_dialog.open()
            }
        }

        CustomToolBarButton {
            anchors.margins: 10
            iconText: FontAwesome.trash
            toolTipText: qsTr("Delete Category")
            enabled: category_box.model.length > 0
            onClicked: {
                delete_dialog.x = category_box.x
                delete_dialog.y = 0
                delete_dialog.element = audio_editor.currentProject.currentCategory
                delete_dialog.mode = 1
                delete_dialog.open()
            }
        }

        CustomToolBarComboBox {
            id: scenario_box
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 150
            emptyString: qsTr("No Scenarios")
            textRole: "name"

            model: {
                audio_editor && audio_editor.currentProject
                        && audio_editor.currentProject.currentCategory ? audio_editor.currentProject.currentCategory.scenarios : []
            }

            currentIndex: {
                audio_editor && audio_editor.currentProject
                        && audio_editor.currentProject.currentCategory ? audio_editor.currentProject.currentCategory.scenarioIndex : -1
            }

            onCurrentTextChanged: {
                if (audio_editor) {
                    audio_editor.setCurrentScenario(currentIndex)
                }
            }
        }

        CustomToolBarButton {
            anchors.margins: 10
            iconText: FontAwesome.pen
            toolTipText: qsTr("Rename Scenario")
            enabled: scenario_box.model.length > 0
            onClicked: {
                rename_dialog.mode = 2
                rename_dialog.title = qsTr("Rename Scenario")
                rename_dialog.origName = scenario_box.currentText
                rename_dialog.x = scenario_box.x
                rename_dialog.y = 0
                rename_dialog.open()
            }
        }

        CustomToolBarButton {
            anchors.margins: 10
            iconText: FontAwesome.trash
            toolTipText: qsTr("Delete Scenario")
            enabled: scenario_box.model.length > 0
            onClicked: {
                delete_dialog.x = scenario_box.x
                delete_dialog.y = 0
                delete_dialog.element = audio_editor.currentProject.currentScenario
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
            toolTipText: qsTr("Sort elements alphabetically")
            onClicked: audio_editor.sortElements()
        }

        CustomToolBarButton {
            iconText: FontAwesome.fileAudio
            iconColor: "darkred"

            toolTipText: qsTr("Remove missing files")

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
