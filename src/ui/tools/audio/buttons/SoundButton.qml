import QtQuick 2.9
import QtQuick.Controls 2.2

import FontAwesome 2.0
import gm.companion.colorscheme 1.0

Rectangle {
    property var element
    property var element_icon
    signal clicked(string element)

    objectName: element
    width: parent ? parent.width : 0
    height: 40

    ColorScheme {
        id: color_scheme
    }

    Image {
        id: icon
        source: element_icon
        width: height
        height: parent.height - 10
        x: 5
        anchors.verticalCenter: parent.verticalCenter

        sourceSize.width: width
        sourceSize.height: height
    }

    Text {
        id: text
        text: element
        color: "black"
        x: icon.width + 10
        y: 5
        width: parent.width - icon.width - 10 - x_button.width
        clip: true
        elide: Text.ElideRight
        font.pointSize: 12

        anchors.verticalCenter: parent.verticalCenter
    }

    color: "transparent"

    Button {
        x: parent.width - width - 5
        id: x_button
        hoverEnabled: true
        height: parent.height - 10
        width: height
        anchors.verticalCenter: parent.verticalCenter

        background: Rectangle {
            color: "transparent"
        }

        Text {
            text: FontAwesome.times
            font.pixelSize: parent.height - 10
            font.family: FontAwesome.familySolid
            anchors.centerIn: parent
            color: parent.pressed ? "darkgrey" : parent.hovered ? "grey" : color_scheme.primaryButtonColor
        }

        onClicked: parent.clicked(parent.element)
    }
}
