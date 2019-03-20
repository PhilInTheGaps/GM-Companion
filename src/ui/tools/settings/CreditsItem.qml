import QtQuick 2.9
import QtQuick.Controls 2.3

Rectangle {
    id: root
    property string title: ""
    property string description: ""
    property var links: []

    height: column.height
    anchors.left: parent.left
    anchors.right: parent.right

    Column {
        id: column
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 10
        padding: 10

        Label {
            id: title_label

            text: root.title
            font.bold: true
            color: color_scheme.textColor
        }

        Label {
            text: root.description
            color: color_scheme.textColor
            wrapMode: Text.WordWrap
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 10
        }

        Row {
            id: link_row
            spacing: 5

            Repeater {
                model: root.links

                Button {
                    text: modelData[0]
                    onClicked: Qt.openUrlExternally(modelData[1])
                }
            }
        }
    }

    color: "transparent"
    border.color: "grey"
    border.width: 1
}
