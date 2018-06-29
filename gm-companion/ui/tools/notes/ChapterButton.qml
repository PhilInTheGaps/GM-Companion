import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4

import gm.companion.colorscheme 1.0

Rectangle {
    property var chapter
    property var parent_width

    signal clicked(string chapter_name)

    width: parent ? parent.width : 0
    height: platform.isAndroid ? width / 6 : 40

    ColorScheme {
        id: colors
    }

    Row {
        anchors.fill: parent
        padding: 5

        Text {
            text: chapter
            color: mouse_area.pressed ? "black" : colors.buttonTextColor
            width: parent.width - parent.padding * 2
            clip: true
            elide: Text.ElideRight
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    color: mouse_area.pressed ? "white" : colors.menuColor

    MouseArea {
        anchors.fill: parent
        id: mouse_area

        onClicked: parent.clicked(parent.chapter)
    }
}
