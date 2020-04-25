import QtQuick 2.9
import QtQuick.Controls 2.2
import "../../../defines.js" as Defines
import FontAwesome 2.0

Rectangle {
    id: root
    height: Defines.TOOLBAR_HEIGHT

    property string text: ""
    property int type: -1

    visible: text !== ""

    onTypeChanged: {
        switch (root.type) {
        case 0:
            icon_label.text = FontAwesome.infoCircle
            icon_label.color = "deepskyblue"
            break
        case 1:
            icon_label.text = FontAwesome.checkCircle
            icon_label.color = "green"
            break
        case 2:
            icon_label.text = FontAwesome.exclamationTriangle
            icon_label.color = "orange"
            break
        case 3:
            icon_label.text = FontAwesome.exclamationCircle
            icon_label.color = "red"
            break
        default:
            break
        }
    }

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
        }

        Label {
            id: text_label
            text: root.text
            font.pointSize: 12
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    border.color: {
        switch (root.type) {
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
