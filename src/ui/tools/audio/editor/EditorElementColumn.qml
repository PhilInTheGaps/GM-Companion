import QtQuick 2.9
import QtQuick.Controls 2.2

import "../buttons"

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
                    anchors.left: parent.left
                    anchors.right: parent.right
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
                        anchors.right: parent.right
                        anchors.margins: 2
                        elide: Text.ElideRight
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            element_stack_view.replace()
                            subscenario_combo_box.isSetEnabled = false
                            audio_editor.loadElement(modelData.name(), 4, "")
                            element_stack_view.replace(empty_page)
                            subscenario_combo_box_rect.visible = false
                            element_info_row_1.visible = false
                            subscenario_combo_box.currentIndex = 0
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
                    }
                }
            }
        }
    }
}
