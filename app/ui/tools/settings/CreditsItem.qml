import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0

Rectangle {
    id: root
    property string title: ""
    property string description: ""
    property var links: []

    height: column.height + 20
    anchors.left: parent.left
    anchors.right: parent.right

    Column {
        id: column
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10
        spacing: 10

        Label {
            id: title_label
            text: root.title
            font.bold: true
        }

        Label {
            text: root.description
            wrapMode: Text.WordWrap
            anchors.left: parent.left
            anchors.right: parent.right
        }

        Flow {
            id: link_row
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 10

            Repeater {
                model: root.links

                CustomButton {
                    buttonText: modelData[0]
                    onClicked: Qt.openUrlExternally(modelData[1])
                    usesFixedWidth: false
                    pointSize: 10
                    padding: 10
                }
            }
        }
    }

    color: "transparent"
    border.color: palette.button
    border.width: 1
}
