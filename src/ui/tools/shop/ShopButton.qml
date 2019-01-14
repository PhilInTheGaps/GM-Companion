import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

Rectangle {
    property var shop

    signal clicked(string shop)

    width: parent ? parent.width : 0
    height: platform.isAndroid ? width / 6 : 40

    Row {
        id: text_row
        anchors.fill: parent
        padding: 5

        Text {
            id: text
            text: shop
            color: mouse_area.pressed ? "black" : color_scheme.buttonTextColor
            width: parent.width - parent.padding * 2
            anchors.verticalCenter: parent.verticalCenter
            clip: true
            elide: Text.ElideRight
        }
    }

    color: mouse_area.pressed ? "white" : color_scheme.primaryButtonColor

    MouseArea {
        anchors.fill: parent
        id: mouse_area

        onClicked: parent.clicked(parent.shop)
    }
}
