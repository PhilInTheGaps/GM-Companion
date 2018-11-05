import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import gm.companion.colorscheme 1.0
import gm.companion.platforms 1.0

Rectangle {
    property var scenario
    property string buttonId
    signal clicked(string scenario)

    width: text.width > 100 ? text.width + 10 : 100
    height: platform.isAndroid ? (parent ? parent.parent.height / 15 : 0) : 40

    ColorScheme {
        id: color_scheme
    }

    PlatformDetails {
        id: platform
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