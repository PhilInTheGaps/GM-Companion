import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3

Rectangle {
    id: page_button
    property var display_text
    property var source

    signal clicked(string source)

    width: row.width
    height: platform.isAndroid ? width / 6 : 40

    Row {
        id: row
        anchors.centerIn: parent
        padding: 5

        Text {
            text: display_text
            color: mouse_area.pressed ? "white" : "black"

            anchors.verticalCenter: parent.verticalCenter
        }
    }

    color: mouse_area.pressed ? "#262f31" : "lightgrey"

    MouseArea {
        anchors.fill: parent
        id: mouse_area

        onClicked: parent.clicked(parent.source)
    }
}
