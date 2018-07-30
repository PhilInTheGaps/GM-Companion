import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import gm.companion.colorscheme 1.0

Rectangle {
    property var shop

    signal clicked(string shop)

    width: parent ? parent.width : 0
    height: platform.isAndroid ? width / 6 : 40

    ColorScheme {
        id: colors
    }

    Row {
        id: text_row
        anchors.fill: parent
        padding: 5

        Text {
            id: text
            text: shop
            color: mouse_area.pressed ? "black" : colors.buttonTextColor
            width: parent.width - parent.padding * 2
            anchors.verticalCenter: parent.verticalCenter
            clip: true
            elide: Text.ElideRight
        }
    }

    color: mouse_area.pressed ? "white" : colors.primaryButtonColor

    MouseArea {
        anchors.fill: parent
        id: mouse_area

        onClicked: parent.clicked(parent.shop)
    }
}
