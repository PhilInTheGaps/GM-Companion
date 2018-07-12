import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4

import "../../fontawesome"
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

        Image {
            id: image
            width: (parent.height - parent.padding * 2) * 0.8
            height: width
            sourceSize.width: width
            sourceSize.height: height
            anchors.verticalCenter: parent.verticalCenter

            Component.onCompleted: {
                switch (type) {
                case 0:
                    source = "/icons/media/music_bright.png"
                    break
                case 1:
                    source = "/icons/media/sound_bright.png"
                    break
                case 2:
                    source = "/icons/media/radio.png"
                    break
                }
            }
        }

        Text {
            text: element
            font.pointSize: 10
            color: mouse_area.pressed ? "black" : color_scheme.buttonTextColor
            width: parent.width - parent.spacing * 3 - up_down_column.width
                   - parent.padding * 2 - image.width - delay_button.width
            clip: true
            elide: Text.ElideRight
            anchors.verticalCenter: parent.verticalCenter
        }

        Column {
            id: up_down_column
            height: parent.height - parent.padding * 2
            width: height
            anchors.verticalCenter: parent.verticalCenter

            Button {
                height: parent.height / 2
                width: parent.width

                background: Rectangle {
                    color: "transparent"
                }

                Icon {
                    icon: icons.fa_angle_up
                    pointSize: 18
                    anchors.centerIn: parent
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                }

                ToolTip.text: qsTr("Move Up")
                ToolTip.visible: hovered
                hoverEnabled: true

                onClicked: moveUp(element_button.element, element_button.type)
            }

            Button {
                height: parent.height / 2
                width: parent.width

                background: Rectangle {
                    color: "transparent"
                }

                Icon {
                    icon: icons.fa_angle_down
                    pointSize: 18
                    anchors.centerIn: parent
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                }

                ToolTip.text: qsTr("Move Down")
                ToolTip.visible: hovered
                hoverEnabled: true

                onClicked: moveDown(element_button.element, element_button.type)
            }
        }

        DelayButton {
            id: delay_button
            delay: 1200

            width: (parent.height - parent.padding * 2)
            height: width

            ToolTip.text: qsTr("Delete ") + element
            ToolTip.visible: hovered
            hoverEnabled: true

            background: Rectangle {
                color: "transparent"
            }

            Icon {
                icon: icons.fa_times
                pointSize: 18
                anchors.centerIn: parent
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
