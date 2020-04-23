import QtQuick 2.9
import QtQuick.Controls 2.2

Rectangle {
    id: page_button
    property var display_text
    property var source

    signal clicked(string source)

    width: row.width
    height: platform.isAndroid ? width / 6 : parent ? parent.height - parent.padding * 2 : 0

    color: "transparent"

    Row {
        id: row
        anchors.centerIn: parent
        padding: 5

        Text {
            text: display_text
            color: mouse_area.pressed ? "grey" : mouse_area.containsMouse ? "lightgrey" : palette.text
            font.bold: true
            font.pointSize: 12
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    MouseArea {
        anchors.fill: parent
        id: mouse_area
        hoverEnabled: true

        onClicked: parent.clicked(parent.source)
    }
}
