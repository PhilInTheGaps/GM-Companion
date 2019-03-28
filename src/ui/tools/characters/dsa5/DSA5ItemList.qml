import QtQuick 2.9
import QtQuick.Controls 2.2

ListView {
    id: root
    anchors.topMargin: 5
    anchors.bottomMargin: 5

    clip: true

    property int value_fields: 0
    property var spacer_indices: [-1]

    ScrollBar.vertical: ScrollBar {
    }

    delegate: Column {
        id: delegate_root
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10

        property var values: modelData.values

        Item {
            anchors.left: parent.left
            anchors.right: parent.right
            height: 40

            Label {
                id: name_label
                text: modelData.name
                color: color_scheme.textColor
                font.pixelSize: parent.height - 25
                font.bold: true
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
            }

            Rectangle {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: name_label.right
                anchors.right: group_label.left
                anchors.margins: 20
                height: 1
                color: color_scheme.dividerColor
            }

            Label {
                id: group_label
                text: modelData.group
                color: color_scheme.textColor
                font.pixelSize: parent.height - 25
                anchors.right: value_row.left
                anchors.margins: 20
                anchors.verticalCenter: parent.verticalCenter
            }

            Row {
                id: value_row
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                spacing: 10

                Repeater {
                    model: root.value_fields

                    Rectangle {
                        border.width: index == 0 ? 1 : 2
                        border.color: "grey"
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.margins: 5
                        width: height
                        color: "transparent"

                        Label {
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            color: color_scheme.textColor

                            text: delegate_root.values[index]
                            font.pixelSize: height - 15
                            font.bold: index == 0
                        }
                    }
                }
            }
        }

        Item {
            visible: root.spacer_indices.includes(index)
            anchors.left: parent.left
            anchors.right: parent.right

            height: 40

            Rectangle {
                height: 10
                width: height
                radius: 5
                color: "transparent"
                border.width: 1
                border.color: color_scheme.dividerColor
                anchors.centerIn: parent
            }
        }
    }
}
