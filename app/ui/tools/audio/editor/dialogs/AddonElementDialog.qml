pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import CustomComponents
import src
import "../../../.."
import "../../buttons"

Dialog {
    id: root

    title: switch (mode) {
           case AddonElementDialog.Mode.Project:
               return qsTr("Add Project From Addon")
           case AddonElementDialog.Mode.Category:
               return qsTr("Add Category From Addon")
           case AddonElementDialog.Mode.Scenario:
               return qsTr("Add Scenario From Addon")
           case AddonElementDialog.Mode.Music:
               return qsTr("Add Music List From Addon")
           case AddonElementDialog.Mode.Sound:
               return qsTr("Add Sound List From Addon")
           case AddonElementDialog.Mode.Radio:
               return qsTr("Add Radio From Addon")
           }

    modal: true
    standardButtons: Dialog.Ok | Dialog.Cancel

    enum Mode {
        Project = 0,
        Category = 1,
        Scenario = 2,
        Music = 3,
        Sound = 4,
        Radio = 5
    }

    property int mode: AddonElementDialog.Mode.Project
    property bool asSubscenario: false
    property int subscenarioIndex: -1
    property var selectedElement: undefined

    onOpened: {
        AudioTool.editor.addons.loadData()
        selectedElement = undefined
    }

    onAccepted: {
        let project = AudioTool.editor.addons.projects[addon_project_combo_box.currentIndex]
        let subscenario = asSubscenario ? subscenarioIndex : -1

        switch (mode) {
        case AddonElementDialog.Mode.Project:
            AudioTool.editor.createProjectFromTemplate(project)
            break
        case AddonElementDialog.Mode.Category:
            AudioTool.editor.createCategoryFromTemplate(project.currentCategory)
            break
        case AddonElementDialog.Mode.Scenario:
            AudioTool.editor.createScenarioFromTemplate(project.currentScenario,
                                                    asSubscenario)
            break
        case AddonElementDialog.Mode.Music:
        case AddonElementDialog.Mode.Sound:
        case AddonElementDialog.Mode.Radio:
            AudioTool.editor.createElementFromTemplate(selectedElement, subscenario)
        }
    }

    contentItem: Item {
        Rectangle {
            id: sidebar
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: Sizes.sidebarWidth

            color: palette.dark
            border.color: Colors.border
            border.width: 1

            CustomComboBox {
                id: addon_combo_box
                anchors.left: parent.left
                anchors.right: parent.right
                emptyString: AudioTool.editor.addons.isLoading ? qsTr("loading ...") : qsTr(
                                                                            "No Addons")

                model: AudioTool.editor.addons.addons
                textRole: "shortName"

                onCurrentTextChanged: {
                    AudioTool.editor.addons.currentIndex = currentIndex
                }
            }

            CustomComboBox {
                id: addon_project_combo_box
                anchors.top: addon_combo_box.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                emptyString: qsTr("No Projects")

                model: AudioTool.editor.addons.projects
                textRole: "name"
            }

            Flickable {
                id: flickable
                anchors.top: addon_project_combo_box.bottom
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 5
                clip: true
                contentHeight: flickable_column.height
                contentWidth: -1

                ScrollBar.vertical: ScrollBar {
                    anchors.right: parent.right
                    visible: flickable.contentHeight > flickable.height
                }

                Column {
                    id: flickable_column
                    anchors.left: parent.left
                    anchors.right: parent.right

                    Repeater {
                        model: addon_project_combo_box.currentIndex >= 0
                               ? AudioTool.editor.addons.projects[addon_project_combo_box.currentIndex].categories : []

                        CustomButton {
                            required property AudioProject modelData

                            anchors.left: parent.left
                            anchors.right: parent.right
                            buttonText: modelData.name // qmllint disable missing-property

                            onClicked: {
                                let project = AudioTool.editor.addons.projects[addon_project_combo_box.currentIndex]
                                project.currentCategory = modelData
                            }
                        }
                    }
                }
            }
        }

        Item {
            anchors.left: sidebar.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            // Scenarios
            Flow {
                id: scenario_flow
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 5

                Repeater {
                    id: scenario_repeater

                    model: addon_project_combo_box.currentIndex >= 0
                           ? AudioTool.editor.addons.projects[addon_project_combo_box.currentIndex].currentCategory.scenarios : []

                    CustomButton {
                        required property AudioScenario modelData

                        buttonText: modelData.name // qmllint disable missing-property
                        padding: 10
                        backgroundColor: "transparent"
                        usesFixedWidth: false
                        onClicked: {
                            let project = AudioTool.editor.addons.projects[addon_project_combo_box.currentIndex]
                            let category = project.currentCategory
                            category.currentScenario = modelData
                        }
                    }
                }
            }

            // Elements
            ListView {
                id: list

                readonly property int min_width: 170
                readonly property int count_per_row: parent ? Math.floor(
                                                                  width / min_width) : 1
                readonly property int button_width: (width - verticalScrollBar.width)
                                                    / count_per_row

                anchors.top: scenario_flow.bottom
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 5
                anchors.bottomMargin: 0
                spacing: 10

                clip: true

                model: addon_project_combo_box.currentIndex >= 0
                       && AudioTool.editor.addons.projects[addon_project_combo_box.currentIndex].currentCategory.currentScenario
                        ? AudioTool.editor.addons.projects[addon_project_combo_box.currentIndex].currentCategory.currentScenario.model : []

                ScrollBar.vertical: ScrollBar {
                    id: verticalScrollBar
                    anchors.top: list.top
                    anchors.right: list.right
                    anchors.bottom: list.bottom
                    visible: list.contentHeight > list.height

                    interactive: list.model.length < 2
                }

                delegate: Column {
                    id: scenario_delegate

                    required property AudioScenario modelData
                    required property int index

                    spacing: 5
                    anchors.left: parent ? parent.left : undefined
                    anchors.right: parent ? parent.right : undefined

                    Label {
                        id: subscenario_text
                        text: scenario_delegate.modelData.name // qmllint disable missing-property
                        font.pointSize: 12
                        verticalAlignment: Text.AlignVCenter
                        visible: scenario_delegate.index > 0

                        anchors.right: parent.right
                        anchors.left: parent.left
                        elide: Text.ElideRight
                    }

                    Flow {
                        anchors.left: parent.left
                        anchors.right: parent.right

                        Repeater {
                            model: scenario_delegate.modelData.elements

                            Item {
                                id: element_delegate

                                required property AudioElement modelData

                                width: list.button_width
                                height: width
                                visible: root.mode < 3 || modelData.type === root.mode - 3

                                AudioButton {
                                    id: element_button
                                    element_name: element_delegate.modelData.name // qmllint disable missing-property
                                    element_type: element_delegate.modelData.type
                                    subscenario_name: subscenario_text.text
                                    thumbnail: element_delegate.modelData.thumbnail
                                    width: parent.width - 4
                                    anchors.centerIn: parent
                                    overlay_enabled: false
                                    hover_enabled: root.mode >= 3

                                    Rectangle {
                                        id: dialog_overlay
                                        anchors.fill: parent

                                        visible: element_button.hover_enabled
                                                 && root.selectedElement === element_delegate.modelData

                                        color: "transparent"
                                        border.color: Colors.border
                                        border.width: 5
                                    }

                                    onClicked: root.selectedElement = element_delegate.modelData
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
