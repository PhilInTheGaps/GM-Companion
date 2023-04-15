import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import "../../../../defines.js" as Defines
import "../../../../colors.js" as Colors
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
        if (audio_addon_element_manager) {
            audio_addon_element_manager.loadData()
        }

        selectedElement = undefined
    }

    onAccepted: {
        if (!audio_editor || !audio_addon_element_manager)
            return

        let project = audio_addon_element_manager.projects[addon_project_combo_box.currentIndex]
        let subscenario = asSubscenario ? subscenarioIndex : -1

        switch (mode) {
        case AddonElementDialog.Mode.Project:
            audio_editor.createProjectFromTemplate(project)
            break
        case AddonElementDialog.Mode.Category:
            audio_editor.createCategoryFromTemplate(project.currentCategory)
            break
        case AddonElementDialog.Mode.Scenario:
            audio_editor.createScenarioFromTemplate(project.currentScenario,
                                                    asSubscenario)
            break
        case AddonElementDialog.Mode.Music:
        case AddonElementDialog.Mode.Sound:
        case AddonElementDialog.Mode.Radio:
            audio_editor.createElementFromTemplate(selectedElement, subscenario)
        }
    }

    contentItem: Item {
        Rectangle {
            id: sidebar
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: Defines.SIDEBAR_WIDTH

            color: palette.dark
            border.color: Colors.border
            border.width: 1

            CustomComboBox {
                id: addon_combo_box
                anchors.left: parent.left
                anchors.right: parent.right
                emptyString: audio_addon_element_manager
                             && audio_addon_element_manager.isLoading ? qsTr("loading ...") : qsTr(
                                                                            "No Addons")

                model: audio_addon_element_manager ? audio_addon_element_manager.addons : []
                textRole: "shortName"

                onCurrentTextChanged: {
                    console.log(currentIndex)

                    if (!audio_addon_element_manager)
                        return

                    audio_addon_element_manager.currentIndex = currentIndex
                }
            }

            CustomComboBox {
                id: addon_project_combo_box
                anchors.top: addon_combo_box.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                emptyString: qsTr("No Projects")

                model: audio_addon_element_manager ? audio_addon_element_manager.projects : []
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
                        model: audio_addon_element_manager
                               && addon_project_combo_box.currentIndex
                               >= 0 ? audio_addon_element_manager.projects[addon_project_combo_box.currentIndex].categories : []

                        CustomButton {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            buttonText: modelData.name

                            onClicked: {
                                let project = audio_addon_element_manager.projects[addon_project_combo_box.currentIndex]
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

                    model: audio_addon_element_manager
                           && addon_project_combo_box.currentIndex
                           >= 0 ? audio_addon_element_manager.projects[addon_project_combo_box.currentIndex].currentCategory.scenarios : []

                    CustomButton {
                        buttonText: modelData.name
                        padding: 10
                        backgroundColor: "transparent"
                        usesFixedWidth: false
                        onClicked: {
                            let project = audio_addon_element_manager.projects[addon_project_combo_box.currentIndex]
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

                model: audio_addon_element_manager
                       && addon_project_combo_box.currentIndex >= 0
                       && audio_addon_element_manager.projects[addon_project_combo_box.currentIndex].currentCategory.currentScenario ? audio_addon_element_manager.projects[addon_project_combo_box.currentIndex].currentCategory.currentScenario.model : []

                ScrollBar.vertical: ScrollBar {
                    id: verticalScrollBar
                    anchors.top: list.top
                    anchors.right: list.right
                    anchors.bottom: list.bottom
                    visible: list.contentHeight > list.height

                    interactive: list.model.length < 2
                }

                delegate: Column {
                    spacing: 5
                    anchors.left: parent ? parent.left : undefined
                    anchors.right: parent ? parent.right : undefined

                    Label {
                        id: subscenario_text
                        text: modelData.name
                        font.pointSize: 12
                        verticalAlignment: Text.AlignVCenter
                        visible: index > 0

                        anchors.right: parent.right
                        anchors.left: parent.left
                        elide: Text.ElideRight
                    }

                    Flow {
                        anchors.left: parent.left
                        anchors.right: parent.right

                        Repeater {
                            model: modelData.elements

                            Item {
                                width: list.button_width
                                height: width
                                visible: mode < 3 || modelData.type === mode - 3

                                AudioButton {
                                    id: element_button
                                    element_name: modelData.name
                                    element_type: modelData.type
                                    subscenario_name: subscenario_text.text
                                    thumbnail: modelData.thumbnail
                                    width: parent.width - 4
                                    anchors.centerIn: parent
                                    overlay_enabled: false
                                    hover_enabled: mode >= 3

                                    Rectangle {
                                        id: dialog_overlay
                                        anchors.fill: parent

                                        visible: element_button.hover_enabled
                                                 && selectedElement === modelData

                                        color: "transparent"
                                        border.color: Colors.border
                                        border.width: 5
                                    }

                                    onClicked: selectedElement = modelData
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
