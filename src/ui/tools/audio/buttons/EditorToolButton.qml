import QtQuick 2.9
import QtQuick.Controls 2.2

import FontAwesome 2.0

ToolButton {
    id: root

    property string labeltext
    property alias label: textlabel

    anchors.top: parent.top
    anchors.bottom: parent.bottom
    width: textlabel.width + 10
    hoverEnabled: true

    Text {
        id: textlabel
        text: root.labeltext
        anchors.centerIn: parent
        color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"

        font.pixelSize: parent.height - 20
        font.family: FontAwesome.familySolid
    }

    background: Rectangle {
        color: "transparent"
    }
}
