import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4

Rectangle {
    property var page
    property var parent_width

    signal clicked(string page_name)

    width: parent_width
    height: 30

    Row {
        id: text_row
        anchors.fill: parent
        padding: 5

        Text {
            text: page
            color: mouse_area.pressed ? "black" : "black"
            width: parent.width - parent.padding * 2
            clip: true
            elide: Text.ElideRight
        }
    }

    color: mouse_area.pressed ? "white" : "lightgrey"

    MouseArea {
        anchors.fill: parent
        id: mouse_area

        onClicked: parent.clicked(parent.page)
    }
}