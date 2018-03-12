import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4

Rectangle {
    property var chapter
    property var parent_width

    signal clicked(string chapter_name)

    width: parent_width
    height: 30

    Row {
        anchors.fill: parent
        padding: 5

        Text {
            text: chapter
            color: mouse_area.pressed ? "black" : "white"
        }
    }

    color: mouse_area.pressed ? "white" : "grey"

    MouseArea {
        anchors.fill: parent
        id: mouse_area

        onClicked: parent.clicked(parent.chapter)
    }
}
