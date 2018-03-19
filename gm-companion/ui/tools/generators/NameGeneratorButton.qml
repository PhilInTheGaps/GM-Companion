import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2

Rectangle {
    property var name
    property var category
    property string path
    signal clicked(string category, string scenario)

    width: text.width > 100 ? text.width + 10 : 100
    height: 30

    Text {
        id: text
        text: name
        color: mouse_area.pressed ? "white" : "black"
        x: 5
        y: 5
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    color: mouse_area.pressed ? "#262f31" : "lightgrey"

    MouseArea {
        anchors.fill: parent
        id: mouse_area

        onClicked: parent.clicked(parent.category, parent.name)
    }
}
