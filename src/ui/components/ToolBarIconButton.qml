import QtQuick 2.9
import QtQuick.Controls 2.2

import FontAwesome 2.0

Button {
    id: root

    property string fa_icon: FontAwesome.bars
    property string icon_color: "white"
    property bool darkBackground: true

    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.margins: 5
    width: height

    hoverEnabled: true

    background: Rectangle {
        color: "transparent"
    }

    Text {
        text: fa_icon
        font.family: FontAwesome.familySolid
        font.pixelSize: parent.height - 10
        height: parent.height - 10
        width: height
        anchors.centerIn: parent
        color: parent.pressed ? (darkBackground ? "grey" : "lightgrey") : parent.hovered ? (darkBackground ? "lightgrey" : "grey") : root.icon_color
    }
}
