import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4

import gm.companion.colorscheme 1.0
import gm.companion.platforms 1.0

Rectangle {
    property var category
    property var max_width
    property string buttonId
    signal clicked(string category)

    width: parent ? parent.width : 0
    height: platform.isAndroid ? width / 6 : 40

    ColorScheme {
        id: color_scheme
    }

    PlatformDetails {
        id: platform
    }

    Row {
        anchors.fill: parent
        padding: 5

        Text {
            text: category
            color: mouse_area.pressed ? "black" : color_scheme.buttonTextColor
            width: parent.width - parent.padding * 2
            anchors.verticalCenter: parent.verticalCenter
            clip: true
            elide: Text.ElideRight
        }
    }

    color: mouse_area.pressed ? "white" : color_scheme.primaryButtonColor

    MouseArea {
        anchors.fill: parent
        id: mouse_area

        onClicked: parent.clicked(parent.category)
    }
}
