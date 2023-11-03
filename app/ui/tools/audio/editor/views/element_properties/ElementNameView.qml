pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import src
import "../../../../.."

Item {
    id: root

    property bool enableMoveButtons: true
    property bool enableDelete: true

    signal deleteClicked

    anchors.left: parent.left
    anchors.right: parent.right
    height: Sizes.toolbarHeight

    Connections {
        target: AudioTool.editor

        function onCurrentElementChanged() {
            element_name_field.text = AudioTool.editor.currentElement ? AudioTool.editor.currentElement.name : "" // qmllint disable missing-property
        }
    }

    Dialog {
        id: subscenario_dialog

        title: qsTr("Change Subscenario")
        modal: true

        x: element_name_field.x
        width: element_name_field.width

        standardButtons: Dialog.Cancel

        contentItem: Column {
            spacing: 5

            Button {
                anchors.right: parent.right
                anchors.left: parent.left

                text: qsTr("No Subscenario")

                onClicked: {
                    AudioTool.editor.setSubscenario(-1)
                    subscenario_dialog.accept()
                }
            }

            Repeater {
                model: {
                    if (AudioTool.editor.currentProject
                            && AudioTool.editor.currentProject.currentScenario) {
                        AudioTool.editor.currentProject.currentScenario.scenarios
                    } else {
                        []
                    }
                }

                Button {
                    required property AudioScenario modelData
                    required property int index

                    anchors.right: parent ? parent.right : undefined
                    anchors.left: parent ? parent.left : undefined

                    text: modelData.name // qmllint disable missing-property

                    onClicked: {
                        AudioTool.editor.setSubscenario(index)
                        subscenario_dialog.accept()
                    }
                }
            }
        }
    }

    Item {
        id: element_up_down
        visible: root.enableMoveButtons
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: height / 2

        Button {
            id: element_up
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height / 2

            background: Rectangle {
                color: palette.alternateBase
            }

            Label {
                text: element_name_field.edit_mode ? FontAwesome.circleCheck : FontAwesome.chevronUp
                font.family: FontAwesome.fontSolid.family
                font.styleName: FontAwesome.fontSolid.styleName
                color: element_name_field.edit_mode ? "limegreen" : palette.text
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            onClicked: {
                if (element_name_field.edit_mode) {
                    element_name_field.edit_mode = false

                    if (AudioTool.editor.currentElement) {
                        AudioTool.editor.currentElement.name = element_name_field.text
                    }

                    element_up_down.visible = enableMoveButtons
                } else {
                    AudioTool.editor.moveElement(AudioTool.editor.currentElement, -1)
                }
            }
        }

        Button {
            id: element_down
            anchors.top: element_up.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            background: Rectangle {
                color: palette.alternateBase
            }

            Label {
                text: element_name_field.edit_mode ? FontAwesome.circleXmark : FontAwesome.chevronDown
                font.family: FontAwesome.fontSolid.family
                font.styleName: FontAwesome.fontSolid.styleName
                color: element_name_field.edit_mode ? "red" : palette.text
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            onClicked: {
                if (element_name_field.edit_mode) {
                    element_name_field.edit_mode = false
                    element_name_field.text = AudioTool.editor.currentElement ? AudioTool.editor.currentElement.name : "" // qmllint disable missing-property
                    element_up_down.visible = root.enableMoveButtons
                } else {
                    AudioTool.editor.moveElement(AudioTool.editor.currentElement, 1)
                }
            }
        }
    }

    CustomToolBarButton {
        id: scenario_button
        width: height / 2
        anchors.margins: 0
        anchors.left: element_up_down.visible ? element_up_down.right : parent.left

        backgroundColor: palette.alternateBase
        transparentBackground: false
        padding: 0
        borderWidth: 0

        iconText: FontAwesome.ellipsisVertical
        toolTipText: qsTr("Change subscenario")

        onClicked: subscenario_dialog.open()
    }

    TextField {
        id: element_name_field
        text: AudioTool.editor.currentElement ? AudioTool.editor.currentProject.name : "" // qmllint disable missing-property
        anchors.left: scenario_button.visible ? scenario_button.right : parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        selectByMouse: true

        property bool edit_mode: false
        activeFocusOnPress: edit_mode
        focus: edit_mode

        rightPadding: leftPadding
                      + (element_delete_button.visible ? element_delete_button.width : 0)
                      + (element_edit_name_button.visible ? element_edit_name_button.width : 0)

        CustomToolBarButton {
            id: element_edit_name_button
            anchors.right: root.enableDelete ? element_delete_button.left : parent.right
            anchors.margins: 0
            visible: !element_name_field.edit_mode

            iconText: FontAwesome.pen
            iconFont: FontAwesome.fontSolid
            pointSize: 12

            onClicked: {
                element_up_down.visible = true
                element_name_field.edit_mode = true
            }
        }

        Button {
            id: element_delete_button
            visible: !element_name_field.edit_mode && root.enableDelete
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            width: height

            background: Rectangle {
                color: "transparent"
            }

            Label {
                text: FontAwesome.trash
                font.family: FontAwesome.fontSolid.family
                font.styleName: FontAwesome.fontSolid.styleName
                font.pointSize: 12
                anchors.fill: parent
                anchors.margins: 10
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            onClicked: element_delete_overlay.visible = true

            Rectangle {
                id: element_delete_overlay
                visible: false
                anchors.fill: parent
                color: palette.alternateBase

                Label {
                    text: FontAwesome.trash
                    font.family: FontAwesome.fontSolid.family
                    font.styleName: FontAwesome.fontSolid.styleName
                    font.pixelSize: parent.height / 3
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    width: height / 2
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                Button {
                    anchors.top: parent.top
                    anchors.right: parent.right
                    width: parent.width / 2
                    height: parent.height / 2

                    background: Rectangle {
                        color: "transparent"
                    }

                    Label {
                        text: FontAwesome.circleCheck
                        font.family: FontAwesome.fontSolid.family
                        font.styleName: FontAwesome.fontSolid.styleName
                        color: "limegreen"
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    onClicked: {
                        root.deleteClicked()
                        element_delete_overlay.visible = false

                        AudioTool.editor.deleteCurrentElement()
                    }
                }

                Button {
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    width: parent.width / 2
                    height: parent.height / 2

                    background: Rectangle {
                        color: "transparent"
                    }

                    Label {
                        text: FontAwesome.circleXmark
                        font.family: FontAwesome.fontSolid.family
                        font.styleName: FontAwesome.fontSolid.styleName
                        color: "red"
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    onClicked: {
                        element_delete_overlay.visible = false
                    }
                }
            }
        }
    }
}
