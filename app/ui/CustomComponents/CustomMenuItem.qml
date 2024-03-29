import QtQuick
import QtQuick.Controls

MenuItem {
    id: root

    property color textColor: palette.text

    contentItem: Text {
        leftPadding: root.indicator ? root.indicator.width : 0
        rightPadding: root.arrow ? root.arrow.width : 0
        text: root.text
        font: root.font
        opacity: enabled ? 1.0 : 0.3
        color: root.highlighted ? palette.highlightedText : root.textColor
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
}
