import QtQuick 2.9
import QtQuick.Controls 2.2

import FontAwesome 2.0

Button {
    id: root

    property string fa_icon: FontAwesome.bars
    property string fa_font: FontAwesome.familySolid
    property string icon_color: "white"
    property string button_text: ""
    property bool darkBackground: true

    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.margins: 5
    width: row.width

    hoverEnabled: true

    background: Rectangle {
        color: "transparent"
    }

    Row {
        id: row
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        spacing: 5

        Text {
            id: icon_text
            text: fa_icon
            font.family: fa_font
            font.pixelSize: parent.height - 10
            height: parent.height - 10
            width: height
            anchors.verticalCenter: parent.verticalCenter

            color: root.pressed ? (darkBackground ? "grey" : "lightgrey") : root.hovered ? (darkBackground ? "lightgrey" : "grey") : root.icon_color
        }

        Text {
            id: description_text
            text: button_text
            visible: text != ""
            anchors.verticalCenter: parent.verticalCenter
            color: icon_text.color
        }
    }
}
