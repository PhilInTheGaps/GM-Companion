import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4

Rectangle {
    property var scenario
    property string buttonId
    signal clicked(string scenario)

    width: text.width > 100 ? text.width + 10 : 100
    height: 30

    Text {
        id: text
        text: scenario
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

        onClicked: parent.clicked(parent.scenario)
    }
}
