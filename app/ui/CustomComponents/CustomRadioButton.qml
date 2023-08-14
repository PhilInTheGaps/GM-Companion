import QtQuick 2.9
import QtQuick.Controls 2.2
import IconFonts

RadioButton {
    id: root
    checked: true
    hoverEnabled: true

    property string iconText: ""
    property var iconFont: FontAwesome.fontSolid

    contentItem: Row {
        leftPadding: root.indicator.width + root.spacing
        spacing: 5

        Label {
            visible: text !== ""
            text: root.iconText
            font.family: root.iconFont.family
            font.styleName: root.iconFont.styleName
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
