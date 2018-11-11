import QtQuick 2.9
import QtQuick.Controls 2.2

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
        font.bold: true
        font.pointSize: 12
    }

    background: Rectangle {
        color: "transparent"
    }
}
