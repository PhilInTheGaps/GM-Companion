import QtQuick 2.9
import QtQuick.Controls 2.3

Rectangle {
    property alias text: text
    property var buttonText
    signal clicked(string info)

    Text {
        id: text
        text: buttonText
        color: mouse_area.pressed ? "black" : mouse_area.hovered ? "lightgrey" : "white"

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5
        anchors.verticalCenter: parent.verticalCenter

        clip: true
        elide: Text.ElideRight
    }

    color: mouse_area.pressed ? "lightgrey" : color_scheme.primaryButtonColor

    MouseArea {
        anchors.fill: parent
        id: mouse_area
        hoverEnabled: true

        onClicked: parent.clicked(parent.buttonText)
    }
}
