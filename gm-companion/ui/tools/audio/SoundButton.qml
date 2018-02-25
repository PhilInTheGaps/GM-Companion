import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4

Rectangle {
    property var element
    property var element_icon
    signal clicked(string element)

    objectName: element
    width: parent.width
    height: 30

    Image {
        id: icon
        source: element_icon
        width: parent.height * 0.8
        height: width
        x: 5
        anchors.verticalCenter: parent.verticalCenter

        sourceSize.width: width
        sourceSize.height: height
    }

    Text {
        id: text
        text: element
        color: mouse_area.pressed ? "white" : "black"
        x: icon.width + 10
        y: 5
        width: parent.width - icon.width - 10
        clip: true
        elide: Text.ElideRight

        anchors.verticalCenter: parent.verticalCenter
    }

    color: mouse_area.pressed ? "#262f31" : "lightgrey"

    MouseArea {
        anchors.fill: parent
        id: mouse_area

        onClicked: parent.clicked(parent.element)
    }
}
