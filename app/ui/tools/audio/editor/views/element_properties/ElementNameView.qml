import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0

import "../../../../../colors.js" as Colors
import "../../../../../sizes.js" as Sizes

Item {
    id: root
    anchors.left: parent.left
    anchors.right: parent.right
    height: Sizes.toolbarHeight

    Connections {
        target: audio_editor

        function onCurrentElementChanged() {
            element_name_field.text = audio_editor
                    && audio_editor.currentElement ? audio_editor.currentElement.name : ""
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
                    audio_editor.setSubscenario(-1)
                    subscenario_dialog.accept()
                }
            }

            Repeater {
                model: {
                    if (audio_editor && audio_editor.currentProject
                            && audio_editor.currentProject.currentScenario) {
                        audio_editor.currentProject.currentScenario.scenarios
                    } else {
                        []
                    }
                }

                Button {
                    anchors.right: parent.right
                    anchors.left: parent.left

                    text: modelData.name

                    onClicked: {
                        audio_editor.setSubscenario(index)
                        subscenario_dialog.accept()
                    }
                }
            }
        }
    }

    Item {
        id: element_up_down
        visible: enable_move_buttons
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
                color: Colors.alternateBase
            }

            Label {
                text: element_name_field.edit_mode ? FontAwesome.checkCircle : FontAwesome.chevronUp
                font.family: FontAwesome.familySolid
                color: element_name_field.edit_mode ? "limegreen" : Colors.text
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            onClicked: {
                if (element_name_field.edit_mode) {
                    element_name_field.edit_mode = false

                    if (audio_editor && audio_editor.currentElement) {
                        audio_editor.currentElement.name = element_name_field.text
                    }

                    element_up_down.visible = enable_move_buttons
                } else {
                    audio_editor.moveElement(audio_editor.currentElement, -1)
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
                color: Colors.alternateBase
            }

            Label {
                text: element_name_field.edit_mode ? FontAwesome.timesCircle : FontAwesome.chevronDown
                font.family: FontAwesome.familySolid
                color: element_name_field.edit_mode ? "red" : Colors.text
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            onClicked: {
                if (element_name_field.edit_mode) {
                    element_name_field.edit_mode = false
                    element_name_field.text = audio_editor
                            && audio_editor.currentElement ? audio_editor.currentElement.name : ""
                    element_up_down.visible = enable_move_buttons
                } else {
                    audio_editor.moveElement(audio_editor.currentElement, 1)
                }
            }
        }
    }

    CustomToolBarButton {
        id: scenario_button
        width: height / 2
        anchors.margins: 0
        anchors.left: element_up_down.visible ? element_up_down.right : parent.left

        backgroundColor: Colors.alternateBase
        transparentBackground: false
        padding: 0
        borderWidth: 0

        iconText: FontAwesome.ellipsisV
        toolTipText: qsTr("Change subscenario")

        onClicked: subscenario_dialog.open()
    }

    TextField {
        id: element_name_field
        text: audio_editor
              && audio_editor.currentElement ? audio_editor.currentProject.name : ""
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
            anchors.right: enable_delete ? element_delete_button.left : parent.right
            anchors.margins: 0
            visible: !element_name_field.edit_mode

            iconText: FontAwesome.pen
            iconFont: FontAwesome.familySolid
            pointSize: 12

            onClicked: {
                element_up_down.visible = true
                element_name_field.edit_mode = true
            }
        }

        Button {
            id: element_delete_button
            visible: !element_name_field.edit_mode && enable_delete
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            width: height

            background: Rectangle {
                color: "transparent"
            }

            Label {
                text: FontAwesome.trashAlt
                font.family: FontAwesome.familySolid
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
                color: Colors.alternateBase

                Label {
                    text: FontAwesome.trashAlt
                    font.family: FontAwesome.familySolid
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
                        text: FontAwesome.checkCircle
                        font.family: FontAwesome.familySolid
                        color: "limegreen"
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    onClicked: {
                        deleteClicked()
                        element_delete_overlay.visible = false

                        audio_editor.deleteCurrentElement()
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
                        text: FontAwesome.timesCircle
                        font.family: FontAwesome.familySolid
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
