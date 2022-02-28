import QtQuick 2.6
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0

import "./buttons"
import "../../defines.js" as Defines

Item {
    id: audio_scenario_element_column

    // Scenarios
    Flow {
        id: scenario_flow
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        Repeater {
            id: scenario_repeater

            model: audio_tool && audio_tool.currentProject
                   && audio_tool.currentProject.currentCategory ? audio_tool.currentProject.currentCategory.scenarios : []

            CustomButton {
                buttonText: modelData.name
                padding: 10
                backgroundColor: "transparent"
                usesFixedWidth: false
                onClicked: {
                    var project = audio_tool.currentProject
                    if (project && project.currentCategory) {
                        project.currentCategory.currentScenario = modelData
                    }
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
        readonly property int button_width: (width - verticalScrollBar.width) / count_per_row

        anchors.top: scenario_flow.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5
        spacing: 10

        clip: true
        model: (audio_tool && audio_tool.currentProject
                && audio_tool.currentProject.currentScenario) ? audio_tool.currentProject.currentScenario.model : []

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

                        AudioButton {
                            id: element_button
                            element_name: modelData.name
                            element_type: modelData.type
                            subscenario_name: subscenario_text.text
                            thumbnail: modelData.thumbnail
                            width: parent.width - 4
                            anchors.centerIn: parent

                            onClicked: audio_tool.play(modelData)
                        }
                    }
                }
            }
        }
    }

    // Search
    Rectangle {
        height: Defines.TOOLBAR_HEIGHT
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 5

        // Search Button
        Rectangle {
            id: search_button
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            width: height
            radius: search_field.visible ? 0 : width / 2

            color: palette.button

            Label {
                font.family: FontAwesome.familySolid
                text: search_field.visible ? FontAwesome.times : FontAwesome.search
                anchors.centerIn: parent
                color: search_mouse_area.containsMouse ? "grey" : palette.buttonText
                font.pixelSize: parent.height / 2
            }

            MouseArea {
                id: search_mouse_area
                anchors.fill: parent
                hoverEnabled: true

                onClicked: {
                    search_field.visible = !search_field.visible
                    search_field.clear()

                    if (search_field.visible) {
                        search_field.forceActiveFocus()
                    }
                }
            }
        }

        TextField {
            id: search_field
            anchors.right: search_button.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            visible: false

            placeholderText: qsTr("Search ...")

            scale: visible ? 1.0 : 0.1
            transformOrigin: Item.Right

            Behavior on scale {
                NumberAnimation {
                    duration: 100
                    easing.type: Easing.Linear
                }
            }

            onTextChanged: audio_tool.findElement(text)

            Keys.onEscapePressed: {
                if (search_field.activeFocus) {
                    search_field.clear()
                    search_field.visible = false
                }
            }
        }
    }
}
