import QtQuick 2.9
import QtQuick.Controls 2.2
import "../../../defines.js" as Defines
import FontAwesome 2.0

Rectangle {
    id: root
    height: Defines.TOOLBAR_HEIGHT

    property var status: undefined

    visible: status.message !== ""

    Row {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        spacing: 10
        padding: 10

        Label {
            id: icon_label
            font.family: FontAwesome.familySolid
            font.pointSize: 12
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter

            text: {
                switch (root.status.type) {
                case 1:
                    FontAwesome.checkCircle
                    break
                case 2:
                    FontAwesome.exclamationTriangle
                    break
                case 3:
                    FontAwesome.exclamationCircle
                    break
                default:
                    FontAwesome.infoCircle
                }
            }

            color: {
                switch (root.status.type) {
                case 1:
                    "green"
                    break
                case 2:
                    "orange"
                    break
                case 3:
                    "red"
                    break
                default:
                    "deepskyblue"
                }
            }
        }

        Label {
            id: text_label
            text: root.status.message
            font.pointSize: 12
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    border.color: {
        switch (root.status.type) {
        case 2:
            "orange"
            break
        case 3:
            "red"
            break
        default:
            palette.highlight
        }
    }

    border.width: 1
    color: "transparent"
}
