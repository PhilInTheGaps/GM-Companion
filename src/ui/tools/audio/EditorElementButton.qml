import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import FontAwesome 2.0
import gm.companion.colorscheme 1.0

Rectangle {
    id: element_button

    property var element
    property int type

    signal clicked(string element, int type)
    signal remove(string element, int type)
    signal moveUp(string element, int type)
    signal moveDown(string element, int type)

    width: parent ? parent.width : 0
    height: 40

    ColorScheme {
        id: color_scheme
    }

    Row {
        id: row
        anchors.fill: parent
        padding: 5
        spacing: 5

        // Left icon
        Text {
            id: icon
            visible: true
            font.pixelSize: (parent.height - parent.padding * 3)
            height: width
            anchors.verticalCenter: parent.verticalCenter

            text: type === 0 ? FontAwesome.music : type
                               === 1 ? FontAwesome.drum : type
                                       === 2 ? FontAwesome.broadcastTower : type
                                               === 3 ? FontAwesome.spotify : FontAwesome.question
            color: "white"
            font.family: type === 3 ? FontAwesome.familyBrands : FontAwesome.familySolid
        }

        // Element Name
        Text {
            text: element
            font.pointSize: 10
            color: mouse_area.pressed ? "black" : color_scheme.buttonTextColor
            width: parent.width - parent.spacing * 3 - up_down_column.width
                   - parent.padding * 2 - icon.width - delay_button.width
            clip: true
            elide: Text.ElideRight
            anchors.verticalCenter: parent.verticalCenter
        }

        // Move Buttons
        Column {
            id: up_down_column
            height: parent.height - parent.padding * 2
            width: height / 2
            anchors.verticalCenter: parent.verticalCenter

            // Up
            Button {
                height: parent.height / 2
                width: parent.width

                background: Rectangle {
                    color: "transparent"
                }

                Text {
                    text: FontAwesome.chevronUp
                    font.pixelSize: parent.height
                    font.family: FontAwesome.familySolid
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                }

                ToolTip.text: qsTr("Move Up")
                ToolTip.visible: hovered
                hoverEnabled: true

                onClicked: moveUp(element_button.element, element_button.type)
            }

            // Down
            Button {
                height: parent.height / 2
                width: parent.width

                background: Rectangle {
                    color: "transparent"
                }

                Text {
                    text: FontAwesome.chevronDown
                    font.pixelSize: parent.height
                    font.family: FontAwesome.familySolid
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                }

                ToolTip.text: qsTr("Move Down")
                ToolTip.visible: hovered
                hoverEnabled: true

                onClicked: moveDown(element_button.element, element_button.type)
            }
        }

        // Delete
        DelayButton {
            id: delay_button
            delay: 1200

            height: parent.height - (parent.padding * 2)
            width: height

            ToolTip.text: qsTr("Delete ") + element
            ToolTip.visible: hovered
            hoverEnabled: true

            background: Rectangle {
                color: "transparent"
            }

            Text {
                text: FontAwesome.times
                font.pixelSize: parent.height
                font.family: FontAwesome.familySolid
                color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
            }

            onActivated: element_button.remove(element_button.element,
                                               element_button.type)
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    color: mouse_area.pressed ? "white" : color_scheme.primaryButtonColor

    MouseArea {
        width: parent.width - delay_button.width - up_down_column.width - row.spacing * 2
        height: parent.height
        id: mouse_area

        ToolTip {
            id: tool_tip
            text: element
        }

        onClicked: parent.clicked(parent.element, parent.type)

        hoverEnabled: true

        onEntered: tool_tip.visible = true
        onExited: tool_tip.visible = false
    }
}
