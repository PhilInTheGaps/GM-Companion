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

            model: audio_tool.scenarioNames

            CustomButton {
                buttonText: modelData
                padding: 10
                onClicked: audio_tool.setCurrentScenario(buttonText)
                backgroundColor: "transparent"
                usesFixedWidth: false
            }
        }
    }

    // Elements
    Flickable {
        id: flickable
        anchors.top: scenario_flow.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        anchors.topMargin: 5

        clip: true

        contentWidth: -1
        contentHeight: audio_element_column.implicitHeight

        ScrollBar.vertical: CustomScrollBar {
            id: verticalScrollBar
            parent: flickable.parent
            anchors.top: flickable.top
            anchors.right: flickable.right
            anchors.bottom: flickable.bottom
            visible: flickable.contentHeight > flickable.height
        }

        Column {
            id: audio_element_column
            anchors.margins: 5
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 10

            readonly property int min_width: 170
            readonly property int count_per_row: parent ? Math.floor(
                                                              parent.width / min_width) : 1
            readonly property int button_width: (width - 5 * (count_per_row - 1)) / count_per_row

            Repeater {
                model: elementModel

                Column {
                    width: audio_element_column.width
                    spacing: 10

                    Label {
                        id: subscenario_text
                        text: modelData.name()
                        visible: text != ""
                        font.bold: true
                        font.pointSize: 15
                    }

                    Flow {
                        id: audio_scroll_flow

                        spacing: 5
                        anchors.left: parent.left
                        anchors.right: parent.right

                        Repeater {
                            id: element_repeater
                            model: modelData

                            AudioButton {
                                element_name: modelData.name
                                element_type: modelData.type
                                subscenario_name: subscenario_text.text
                                icon: modelData.icon
                                width: audio_element_column.button_width

                                onClicked: audio_tool.playElement(
                                               modelData.name, modelData.type,
                                               subscenario_name)
                            }
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
