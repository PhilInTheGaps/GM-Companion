import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import IconFonts

Rectangle {
    id: root
    property string title: ""
    property string description: ""
    property var license: []
    property var links: []

    height: Math.max(column.height + 20, licens_column.height)
    anchors.left: parent.left
    anchors.right: parent.right

    Column {
        id: column
        anchors.top: parent.top
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

                Button {
                    text: modelData[0]
                    onClicked: Qt.openUrlExternally(modelData[1])
                }
            }
        }
    }

    Column {
        id: licens_column
        anchors.right: parent.right

        Repeater {
            model: license

            CustomButton {
                buttonText: modelData[0]
                onClicked: Qt.openUrlExternally(modelData[1])
                iconText: FontAwesome.scaleBalanced
                iconFont: FontAwesome.fontSolid
                usesFixedWidth: false
                backgroundColor: "transparent"

                height: 30
                pointSize: 10
                mainRow.padding: 5
            }
        }
    }

    color: "transparent"
    border.color: palette.button
    border.width: 1
}
