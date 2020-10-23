import QtQuick 2.9
import QtQuick.Controls 2.2
import "../../colors.js" as Colors

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
