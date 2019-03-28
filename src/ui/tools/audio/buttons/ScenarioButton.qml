import QtQuick 2.9
import QtQuick.Controls 2.2

import FontAwesome 2.0

Button {
    id: root

    property string labeltext
    property alias label: textlabel

    width: textlabel.width + 20
    height: color_scheme.toolbarHeight
    hoverEnabled: true

    Text {
        id: textlabel
        text: root.labeltext
        anchors.centerIn: parent
        color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
        font.pixelSize: parent.height - 30
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        color: "transparent"
    }
}
