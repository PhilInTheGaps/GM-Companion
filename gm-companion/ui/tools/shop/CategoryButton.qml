import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4

import gm.companion.colorscheme 1.0

Rectangle {
    property var category
    property var parent_width

    signal clicked(string category)

    //    width: parent_width
    width: parent.width
    height: 30

    ColorScheme {
        id: colors
    }

    Row {
        anchors.fill: parent
        padding: 5

        Text {
            text: category
            color: mouse_area.pressed ? "black" : colors.buttonTextColor
        }
    }

    color: mouse_area.pressed ? "white" : colors.menuColor

    MouseArea {
        anchors.fill: parent
        id: mouse_area

        onClicked: parent.clicked(parent.category)
    }
}
