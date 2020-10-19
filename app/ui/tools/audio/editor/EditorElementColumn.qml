import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0

import "../buttons"
import "../../../defines.js" as Defines

Rectangle {
    id: root

    anchors.fill: parent
    color: palette.dark

    ListView {
        id: list

        property bool small_mode: true

        anchors.fill: parent
        anchors.topMargin: 5
        anchors.bottomMargin: 5

        clip: true
        spacing: 10
        model: (audio_editor && audio_editor.currentProject
                && audio_editor.currentProject.currentScenario) ? audio_editor.currentProject.currentScenario.model : []

        ScrollBar.vertical: CustomScrollBar {
            id: scroll_bar
            visible: list.contentHeight > list.height
        }

        delegate: Column {
            id: elements_column

            anchors.left: parent ? parent.left : undefined
            anchors.right: parent ? parent.right : undefined
            anchors.rightMargin: scroll_bar.visible ? 10 : 5
            anchors.leftMargin: 5
            spacing: 5

            Rectangle {
                id: subscenario_rect
                anchors.left: parent.left
                anchors.right: parent.right
                height: subscenario_text.height + 4
                visible: modelData.isSubscenario
                color: palette.button

                Label {
                    id: subscenario_text
                    text: modelData.name
                    color: palette.buttonText
                    font.bold: true

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.right: subscenario_button_row.left
                    anchors.margins: 2
                    elide: Text.ElideRight
                }

                Row {
                    id: subscenario_button_row

                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    anchors.leftMargin: 5
                    anchors.rightMargin: 2
                    spacing: 2

                    CustomToolBarButton {
                        iconText: FontAwesome.chevronUp
                        anchors.margins: 0
                        pointSize: 12
                        padding: 2
                        onClicked: audio_editor.moveSubscenario(modelData, -1)
                    }

                    CustomToolBarButton {
                        iconText: FontAwesome.chevronDown
                        anchors.margins: 0
                        pointSize: 12
                        padding: 2
                        onClicked: audio_editor.moveSubscenario(modelData, 1)
                    }

                    CustomToolBarButton {
                        iconText: FontAwesome.trash
                        anchors.margins: 0
                        pointSize: 10
                        onClicked: {
                            delete_dialog.x = element_column.width
                            delete_dialog.y = Defines.TOOLBAR_HEIGHT
                            delete_dialog.mode = 3
                            delete_dialog.element = modelData
                            delete_dialog.open()
                        }
                    }
                }
            }

            Repeater {
                id: repeater
                model: modelData.elements

                AudioButton {
                    element_name: modelData.name
                    element_type: modelData.type
                    subscenario_name: subscenario_text.text
                    icon: modelData.icon

                    overlay_enabled: false
                    small_mode: list.small_mode

                    anchors.left: parent.left
                    anchors.right: parent.right

                    onClicked: {
                        audio_editor.loadElement(modelData)
                    }
                }
            }
        }
    }
}
