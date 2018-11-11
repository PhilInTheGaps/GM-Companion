import QtQuick 2.9
import QtQuick.Controls 2.2

import gm.companion.colorscheme 1.0
import gm.companion.platforms 1.0

Rectangle {
    property alias text: text
    property var buttonText
    signal clicked(string info)

    ColorScheme {
        id: color_scheme
    }

    Text {
        id: text
        text: buttonText
        padding: 5
        color: mouse_area.pressed ? "black" : color_scheme.buttonTextColor
        width: parent.width - parent.padding * 2
        anchors.verticalCenter: parent.verticalCenter
        clip: true
        elide: Text.ElideRight
    }

    color: mouse_area.pressed ? "white" : color_scheme.primaryButtonColor

    MouseArea {
        anchors.fill: parent
        id: mouse_area

        onClicked: parent.clicked(parent.buttonText)
    }
}
