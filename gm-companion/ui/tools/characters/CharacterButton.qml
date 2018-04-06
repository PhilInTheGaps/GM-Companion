import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4

import gm.companion.colorscheme 1.0

Rectangle {
    property var character_name
    property bool active: true

    signal clicked(string character_name)
    signal toggle_active_clicked(string character_name, bool active)

    width: parent.width
    height: 30

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
            elide: Text.ElideRight
        }
    }

    color: mouse_area.pressed ? "white" : color_scheme.primaryButtonColor

    MouseArea {
        anchors.fill: parent
        id: mouse_area

        onClicked: parent.clicked(parent.character_name)
    }

    Button {
        id: toggle_active_button
        width: parent.height - 10
        height: width

        x: parent.width - width - 5
        anchors.verticalCenter: parent.verticalCenter

        text: parent.active ? "x" : "<"

        onClicked: parent.toggle_active_clicked(parent.character_name,
                                                parent.active)
    }
}
