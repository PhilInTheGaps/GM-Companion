import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2

import gm.companion.colorscheme 1.0

Rectangle {
    property var name
    property var category
    property string path
    signal clicked(string category, string scenario)

    width: text.width > 100 ? text.width + 10 : 100
    height: platform.isAndroid ? text.font.pixelSize * 1.5 : 40

    ColorScheme {
        id: colors
    }

    Text {
        id: text
        text: name
        color: mouse_area.pressed ? "white" : colors.buttonTextColor
        x: 5
        y: 5
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        width: parent.width - parent.padding * 2
        clip: true
        elide: Text.ElideRight
    }

    color: mouse_area.pressed ? "#262f31" : colors.primaryButtonColor

    MouseArea {
        anchors.fill: parent
        id: mouse_area

        onClicked: parent.clicked(parent.category, parent.name)
    }
}
