import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0

import "../buttons"
import "../../../defines.js" as Defines

Flickable {
    id: root

    property bool small_mode: true

    anchors.fill: parent
    anchors.margins: 5

    clip: true
    contentWidth: -1
    contentHeight: elements_column.implicitHeight

    ScrollBar.vertical: CustomScrollBar {
        visible: root.contentHeight > root.height
    }

    Column {
        id: elements_column

        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 10

        Repeater {
            id: repeater
            model: editorElementModel

            Column {
                anchors.left: parent.left
                anchors.right: parent.right
                spacing: 5

                Rectangle {
                    id: subscenario_rect
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    height: subscenario_text.height + 4
                    color: palette.button
                    visible: subscenario_text.text != ""

                    Label {
                        id: subscenario_text
                        text: modelData.name()
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
                        anchors.rightMargin: 5
                        spacing: 5

                        CustomToolBarButton {
                            iconText: FontAwesome.chevronUp
                            anchors.margins: 0
                            onClicked: audio_editor.moveElement(
                                           modelData.name(), 4, -1)
                        }

                        CustomToolBarButton {
                            iconText: FontAwesome.chevronDown
                            anchors.margins: 0
                            onClicked: audio_editor.moveElement(
                                           modelData.name(), 4, 1)
                        }

                        CustomToolBarButton {
                            iconText: FontAwesome.trash
                            anchors.margins: 0
                            onClicked: {
                                delete_dialog.x = element_column.width
                                delete_dialog.y = Defines.TOOLBAR_HEIGHT
                                delete_dialog.mode = 3
                                delete_dialog.element_name = modelData.name()
                                delete_dialog.open()
                            }
                        }
                    }
                }

                Repeater {
                    model: modelData

                    AudioButton {
                        element_name: modelData.name
                        element_type: modelData.type
                        subscenario_name: subscenario_text.text
                        icon: modelData.icon

                        overlay_enabled: false
                        small_mode: root.small_mode

                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.rightMargin: 10

                        onClicked: {
                            audio_editor.loadElement(element_name,
                                                     element_type,
                                                     subscenario_name)
                        }
                    }
                }
            }
        }
    }
}
