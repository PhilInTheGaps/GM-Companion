import QtQuick 2.9
import QtQuick.Controls 2.2
import FontAwesome 2.0

RadioButton {
    id: root
    checked: true
    hoverEnabled: true

    property string iconText: ""
    property var iconFont: FontAwesome.familySolid

    contentItem: Row {
        leftPadding: root.indicator.width + root.spacing
        spacing: 5

        Label {
            visible: text !== ""
            text: root.iconText
            font.family: root.iconFont
            opacity: enabled ? (root.down ? 0.7 : 1.0) : 0.3
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
        }

        Label {
            text: root.text
            font: root.font
            opacity: enabled ? (root.down ? 0.7 : 1.0) : 0.3
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
