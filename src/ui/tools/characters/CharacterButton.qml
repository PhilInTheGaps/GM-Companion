import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import FontAwesome 2.0
import gm.companion.colorscheme 1.0

Rectangle {
    id: character_button
    property var character_name
    property bool active: true

    signal clicked(string character_name)
    signal toggle_active_clicked(string character_name, bool active)

    width: parent ? parent.width : 0
    height: platform.isAndroid ? width / 6 : 40

    ColorScheme {
        id: color_scheme
    }

    Row {
        anchors.fill: parent
        padding: 5

        Text {
            text: character_name
            color: mouse_area.pressed ? "black" : color_scheme.buttonTextColor

            width: toggle_active_button.x - parent.padding - 5
            anchors.verticalCenter: parent.verticalCenter
            clip: true
            elide: Text.ElideRight
        }
    }

    color: mouse_area.pressed ? "white" : color_scheme.primaryButtonColor

    MouseArea {
        anchors.fill: parent
        id: mouse_area
        hoverEnabled: true

        onClicked: parent.clicked(parent.character_name)
    }

    Button {
        id: toggle_active_button
        width: parent.height - 10
        height: width

        x: parent.width - width - 5
        anchors.verticalCenter: parent.verticalCenter

        ToolTip.text: parent.active ? "Make Inactive" : "Make Active"
        ToolTip.visible: hovered
        hoverEnabled: true

        visible: mouse_area.containsMouse || hovered

        background: Rectangle {
            color: "transparent"
        }

        Text {
            text: character_button.active ? FontAwesome.times : FontAwesome.plus
            font.family: FontAwesome.familySolid
            font.pointSize: 15
            anchors.centerIn: parent
            color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
        }

        onClicked: parent.toggle_active_clicked(parent.character_name,
                                                parent.active)
    }
}
