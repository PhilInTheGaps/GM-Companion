import QtQuick
import QtQuick.Controls
import "../.."

TextField {
    property string fieldText: ""

    anchors.top: parent.top
    anchors.bottom: parent.bottom

    selectByMouse: true
    font.pointSize: 12

    text: fieldText
    onTextEdited: {
        fieldText = text
    }

    background: Item {}

    Rectangle {
        color: Colors.border
        height: 1
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 6
    }
}
