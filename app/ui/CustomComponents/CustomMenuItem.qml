import QtQuick 2.9
import QtQuick.Controls 2.2

MenuItem {
    id: root

    property string textColor: palette.text

    hoverEnabled: true

    contentItem: Text {
        leftPadding: root.indicator.width
        rightPadding: root.arrow.width
        text: root.text
        font: root.font
        opacity: enabled ? 1.0 : 0.3
        color: root.highlighted ? palette.highlightedText : root.textColor
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 25
        opacity: enabled ? 1 : 0.3
        color: "transparent"
        border.color: palette.button
        border.width: root.hovered ? 1 : 0
    }
}
