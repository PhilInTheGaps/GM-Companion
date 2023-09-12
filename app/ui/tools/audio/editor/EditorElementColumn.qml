pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import src
import "../buttons"
import "../../.."
import "./dialogs"

Pane {
    id: root

    required property EditorDeleteDialog deleteDialog

    anchors.fill: parent

    ListView {
        id: list

        property bool small_mode: true

        anchors.fill: parent

        clip: true
        spacing: 10
        model: (AudioTool.editor.currentProject
                && AudioTool.editor.currentProject.currentScenario) ? AudioTool.editor.currentProject.currentScenario.model : []

        ScrollBar.vertical: ScrollBar {
            id: scroll_bar
            visible: list.contentHeight > list.height
        }

        delegate: Column {
            id: elements_column

            required property AudioScenario modelData

            anchors.left: parent ? parent.left : undefined
            anchors.right: parent ? parent.right : undefined
            anchors.rightMargin: scroll_bar.visible ? scroll_bar.width : 0
            spacing: 5

            Rectangle {
                id: subscenario_rect
                anchors.left: parent.left
                anchors.right: parent.right
                height: subscenario_text.height + 4
                visible: elements_column.modelData.isSubscenario
                color: palette.button

                Label {
                    id: subscenario_text
                    text: elements_column.modelData.name // qmllint disable missing-property
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
                        onClicked: AudioTool.editor.moveSubscenario(elements_column.modelData, -1)
                    }

                    CustomToolBarButton {
                        iconText: FontAwesome.chevronDown
                        anchors.margins: 0
                        pointSize: 12
                        padding: 2
                        onClicked: AudioTool.editor.moveSubscenario(elements_column.modelData, 1)
                    }

                    CustomToolBarButton {
                        iconText: FontAwesome.trash
                        anchors.margins: 0
                        pointSize: 10
                        onClicked: {
                            root.deleteDialog.x = root.width
                            root.deleteDialog.y = Sizes.toolbarHeight
                            root.deleteDialog.mode = 3
                            root.deleteDialog.element = elements_column.modelData
                            root.deleteDialog.open()
                        }
                    }
                }
            }

            Repeater {
                id: repeater
                model: elements_column.modelData.elements

                AudioButton {
                    required property AudioElement modelData

                    element_name: modelData.name // qmllint disable missing-property
                    element_type: modelData.type
                    subscenario_name: subscenario_text.text
                    thumbnail: modelData.thumbnail

                    overlay_enabled: false
                    small_mode: list.small_mode

                    anchors.left: elements_column.left
                    anchors.right: elements_column.right

                    onClicked: {
                        AudioTool.editor.loadElement(modelData)
                    }
                }
            }
        }
    }
}
