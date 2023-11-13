pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import src
import common
import "./dialogs"
import "../audio_exporter"

CustomToolBar {
    id: root

    required property EditorDeleteDialog deleteDialog
    required property EditorNewThingDialog newThingDialog
    required property EditorRenameDialog renameDialog
    required property AudioExporter exporterDialog

    enableBack: true
    enableAdd: true
    enableSave: true
    enableExport: true

    isSaved: AudioTool.editor.isSaved

    onAddClicked: newThingDialog.open()
    onSaveClicked: AudioTool.editor.saveProject()
    onExportClicked: {
        AudioTool.editor.exporter.project = project_box.currentText;
        exporterDialog.open();
    }

    Row {
        id: row
        anchors.left: root.button_row.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        padding: 10
        spacing: 5

        CustomToolBarComboBox {
            id: project_box

            property int projectIndex: AudioTool.editor.projectIndex

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 150
            textRole: "name"
            emptyString: AudioTool.editor.isLoading ? qsTr("Loading ...") : qsTr("No Projects")

            model: AudioTool.editor.projects

            onCurrentTextChanged: {
                AudioTool.editor.setCurrentProject(currentIndex);
            }

            // If a project is created and set as the current project,
            // change the current index accordingly.
            onProjectIndexChanged: {
                var project = AudioTool.editor.currentProject;
                // qmllint disable missing-property
                if (project && project.name !== project_box.currentText) {
                    project_box.currentIndex = AudioTool.editor.projectIndex;
                }
            }
        }

        CustomToolBarButton {
            anchors.margins: 10
            iconText: FontAwesome.pen
            toolTipText: qsTr("Rename Project")
            enabled: project_box.model.length > 0
            onClicked: {
                root.renameDialog.mode = 0;
                root.renameDialog.title = qsTr("Rename Project");
                root.renameDialog.originalName = project_box.currentText;
                root.renameDialog.x = project_box.x + row.x;
                root.renameDialog.y = 0;
                root.renameDialog.open();
            }
        }

        CustomToolBarButton {
            anchors.margins: 10
            iconText: FontAwesome.trash
            toolTipText: qsTr("Delete Project")
            enabled: project_box.model.length > 0
            onClicked: {
                root.deleteDialog.x = project_box.x + row.x;
                root.deleteDialog.y = 0;
                root.deleteDialog.element = AudioTool.editor.currentProject;
                root.deleteDialog.mode = 0;
                root.deleteDialog.open();
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
                AudioTool.editor.currentProject ? AudioTool.editor.currentProject.categories : [];
            }

            currentIndex: {
                AudioTool.editor.currentProject ? AudioTool.editor.currentProject.categoryIndex : -1;
            }

            onCurrentTextChanged: {
                if (AudioTool.editor.currentProject && AudioTool.editor.currentProject.currentCategory && AudioTool.editor.currentProject.currentCategory.name // qmllint disable missing-property
                !== category_box.currentText) {
                    AudioTool.editor.setCurrentCategory(currentIndex);
                }
            }
        }

        CustomToolBarButton {
            anchors.margins: 10
            iconText: FontAwesome.pen
            toolTipText: qsTr("Rename Category")
            enabled: category_box.model.length > 0
            onClicked: {
                root.renameDialog.mode = 1;
                root.renameDialog.title = qsTr("Rename Category");
                root.renameDialog.originalName = category_box.currentText;
                root.renameDialog.x = category_box.x + row.x;
                root.renameDialog.y = 0;
                root.renameDialog.open();
            }
        }

        CustomToolBarButton {
            anchors.margins: 10
            iconText: FontAwesome.trash
            toolTipText: qsTr("Delete Category")
            enabled: category_box.model.length > 0
            onClicked: {
                root.deleteDialog.x = category_box.x + row.x;
                root.deleteDialog.y = 0;
                root.deleteDialog.element = AudioTool.editor.currentProject.currentCategory;
                root.deleteDialog.mode = 1;
                root.deleteDialog.open();
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
                AudioTool.editor.currentProject && AudioTool.editor.currentProject.currentCategory ? AudioTool.editor.currentProject.currentCategory.scenarios : [];
            }

            currentIndex: {
                AudioTool.editor.currentProject && AudioTool.editor.currentProject.currentCategory ? AudioTool.editor.currentProject.currentCategory.scenarioIndex : -1;
            }

            onCurrentTextChanged: {
                AudioTool.editor.setCurrentScenario(currentIndex);
            }
        }

        CustomToolBarButton {
            anchors.margins: 10
            iconText: FontAwesome.pen
            toolTipText: qsTr("Rename Scenario")
            enabled: scenario_box.model.length > 0
            onClicked: {
                root.renameDialog.mode = 2;
                root.renameDialog.title = qsTr("Rename Scenario");
                root.renameDialog.originalName = scenario_box.currentText;
                root.renameDialog.x = scenario_box.x + row.x;
                root.renameDialog.y = 0;
                root.renameDialog.open();
            }
        }

        CustomToolBarButton {
            anchors.margins: 10
            iconText: FontAwesome.trash
            toolTipText: qsTr("Delete Scenario")
            enabled: scenario_box.model.length > 0
            onClicked: {
                root.deleteDialog.x = scenario_box.x + row.x;
                root.deleteDialog.y = 0;
                root.deleteDialog.element = AudioTool.editor.currentProject.currentScenario;
                root.deleteDialog.mode = 2;
                root.deleteDialog.open();
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
            iconText: FontAwesome.arrowDownAZ
            toolTipText: qsTr("Sort elements alphabetically")
            onClicked: AudioTool.editor.sortElements()
        }

        CustomToolBarButton {
            iconText: FontAwesome.fileAudio
            iconColor: SettingsManager.colors.error

            toolTipText: qsTr("Remove missing files")

            Label {
                text: FontAwesome.trash
                font.family: FontAwesome.fontSolid.family
                font.styleName: FontAwesome.fontSolid.styleName
                font.pixelSize: height

                height: parent.height / 3
                width: height
                x: parent.width - width * 1.5
                y: parent.height - height * 1.5

                color: parent.mouseArea.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"

                background: Rectangle {
                    color: palette.alternateBase
                }
            }

            onClicked: AudioTool.editor.removeMissingFiles(AudioTool.editor.name, AudioTool.editor.type) // qmllint disable missing-property
        }
    }
}
