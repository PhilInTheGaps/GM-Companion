import QtQuick 2.9
import QtQuick.Controls 2.2
import FontAwesome 2.0

import "../buttons"
import "../../../components"

ScrollView {
    id: root

    property bool small_mode: true

    anchors.fill: parent
    anchors.margins: 5

    clip: true
    contentWidth: -1
    contentHeight: elements_column.implicitHeight

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
                    color: color_scheme.primaryButtonColor
                    visible: subscenario_text.text != ""

                    Text {
                        id: subscenario_text
                        text: modelData.name()
                        color: color_scheme.buttonTextColor
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

                        ToolBarIconButton {
                            fa_icon: FontAwesome.chevronUp
                            anchors.margins: 0
                            onClicked: audio_editor.moveElement(
                                           modelData.name(), 4, -1)
                        }

                        ToolBarIconButton {
                            fa_icon: FontAwesome.chevronDown
                            anchors.margins: 0
                            onClicked: audio_editor.moveElement(
                                           modelData.name(), 4, 1)
                        }

                        ToolBarIconButton {
                            fa_icon: FontAwesome.trash
                            anchors.margins: 0
                            onClicked: {
                                delete_dialog.x = element_column.width
                                delete_dialog.y = color_scheme.toolbarHeight
                                delete_dialog.mode = 3
                                delete_dialog.element_name = modelData.name()
                                delete_dialog.open()
                            }
                        }
                    }
                }

                Repeater {
                    model: modelData

                    EditorElementButton {
                        element_name: modelData.name
                        element_type: modelData.type
                        subscenario_name: subscenario_text.text
                        icon: modelData.icon
                        has_icon: modelData.hasIcon
                        small_mode: root.small_mode

                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                    }
                }
            }
        }
    }
}
