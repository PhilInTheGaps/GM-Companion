import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Templates as T
import "./colors.js" as Colors

T.TextArea {
    id: control

    implicitWidth: Math.max(
                       contentWidth + leftPadding + rightPadding,
                       background ? background.implicitWidth : 0,
                       placeholder.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(
                        contentHeight + topPadding + bottomPadding,
                        background ? background.implicitHeight : 0,
                        placeholder.implicitHeight + topPadding + bottomPadding)

    padding: 6
    leftPadding: padding + 4

    color: enabled ? Colors.text : Colors.textDisabled
    selectionColor: Colors.highlight
    selectedTextColor: Colors.highlightedText

    PlaceholderText {
        id: placeholder
        x: control.leftPadding
        y: control.topPadding
        width: control.width - (control.leftPadding + control.rightPadding)
        height: control.height - (control.topPadding + control.bottomPadding)

        text: control.placeholderText
        font: control.font
        color: enabled ? Colors.text : Colors.textDisabled
        opacity: 0.7
        verticalAlignment: control.verticalAlignment
        visible: !control.length && !control.preeditText
                 && (!control.activeFocus
                     || control.horizontalAlignment !== Qt.AlignHCenter)
        elide: Text.ElideRight
    }
}
