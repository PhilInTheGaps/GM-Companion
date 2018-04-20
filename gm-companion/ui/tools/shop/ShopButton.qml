import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4

import gm.companion.colorscheme 1.0

Rectangle {
    property var shop

    signal clicked(string shop)

    //    width: text.width + text_row.padding * 2
    width: parent.width
    height: 30

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
        }
    }

    color: mouse_area.pressed ? "white" : colors.primaryButtonColor

    MouseArea {
        anchors.fill: parent
        id: mouse_area

        onClicked: parent.clicked(parent.shop)
    }
}
