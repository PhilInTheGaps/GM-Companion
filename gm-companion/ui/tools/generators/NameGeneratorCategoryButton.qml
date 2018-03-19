import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4

Rectangle {
    property var category
    property var max_width
    signal clicked(string category)

    width: max_width
    height: 30

    Row {
        anchors.fill: parent
        padding: 5

        Text {
            text: category
            color: mouse_area.pressed ? "black" : "white"
        }
    }

    color: mouse_area.pressed ? "white" : "#262f31"

    MouseArea {
        anchors.fill: parent
        id: mouse_area

        onClicked: parent.clicked(parent.category)
    }
}
