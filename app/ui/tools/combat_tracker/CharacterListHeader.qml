import QtQuick 2.9
import QtQuick.Controls 2.2

Rectangle {
    id: list_header
    color: palette.highlight

    Grid {
        id: grid
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        columns: 4
        rows: 1
        verticalItemAlignment: Grid.AlignVCenter
        padding: 10
        spacing: 10

        Repeater {
            model: labels

            Label {
                text: modelData.text
                font.pointSize: 12
                width: parent.width / modelData.divisor
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: modelData.alignment
                                     === "left" ? Text.AlignLeft : Text.AlignHCenter
            }
        }
    }

    readonly property var labels: [{
            "text": qsTr("Name"),
            "divisor": 5,
            "alignment": "left"
        }, {
            "text": qsTr("Initiative"),
            "divisor": 6,
            "alignment": "center"
        }, {
            "text": qsTr("Health"),
            "divisor": 6,
            "alignment": "center"
        }, {
            "text": qsTr("Notes"),
            "divisor": 6,
            "alignment": "left"
        }]
}
