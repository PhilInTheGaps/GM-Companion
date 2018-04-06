import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4

import gm.companion.colorscheme 1.0

Rectangle {
    property var scenario
    property string buttonId
    signal clicked(string scenario)

    width: text.width > 100 ? text.width + 10 : 100
    height: 30

    ColorScheme {
        id: color_scheme
    }

    Text {
        id: text
        text: scenario
        color: mouse_area.pressed ? color_scheme.buttonTextColor : color_scheme.buttonTextColor
        x: 5
        y: 5
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    color: mouse_area.pressed ? color_scheme.dividerColor : color_scheme.secondaryButtonColor

    MouseArea {
        anchors.fill: parent
        id: mouse_area

        onClicked: parent.clicked(parent.scenario)
    }
}
