import QtQuick 2.9
import QtQuick.Controls 2.3

Rectangle {
    property alias text: text
    property var buttonText
    property string textColor: "white"
    property bool darkBackground: true
    signal clicked(string info)

    Text {
        id: text
        text: buttonText
        color: mouse_area.pressed ? (darkBackground ? "black" : "lightgrey") : (mouse_area.containsMouse ? (darkBackground ? "lightgrey" : "grey") : textColor)
        font.bold: mouse_area.containsMouse

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
