import QtQuick 2.9
import QtQuick.Controls 2.2

Item {
    property string field_text: ""
    property string text_color: "white"

    height: parent.height - parent.padding * 2
    anchors.verticalCenter: parent.verticalCenter

    TextField {
        text: field_text
        anchors.fill: parent
        selectByMouse: true
        onTextEdited: field_text = text

        color: text_color
        font.pointSize: 12

        background: Rectangle {
            color: "transparent"
        }
    }

    Rectangle {
        color: color_scheme.dividerColor
        height: 1
        anchors.left: parent.left
        anchors.right: parent.right
        y: parent.height - 2
    }
}
