import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.TextField {
    id: control

    implicitWidth: Math.max(
                       background ? background.implicitWidth : 0,
                       placeholderText ? placeholder.implicitWidth + leftPadding + rightPadding : 0)
                   || contentWidth + leftPadding + rightPadding
    implicitHeight: Math.max(
                        contentHeight + topPadding + bottomPadding,
                        background ? background.implicitHeight : 0,
                        placeholder.implicitHeight + topPadding + bottomPadding)

    padding: 6
    leftPadding: padding + 4

    color: enabled ? palette.text : StyleColors.textDisabled
    selectionColor: palette.highlight
    selectedTextColor: palette.highlightedText
    verticalAlignment: TextInput.AlignVCenter

    PlaceholderText {
        id: placeholder
        x: control.leftPadding
        y: control.topPadding
        width: control.width - (control.leftPadding + control.rightPadding)
        height: control.height - (control.topPadding + control.bottomPadding)

        text: control.placeholderText
        font: control.font
        color: enabled ? palette.text : StyleColors.textDisabled
        opacity: 0.7
        verticalAlignment: control.verticalAlignment
        visible: !control.length && !control.preeditText
                 && (!control.activeFocus
                     || control.horizontalAlignment !== Qt.AlignHCenter)
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: StyleSizes.textFieldHeight
        border.width: control.activeFocus ? 2 : 1
        color: palette.window
        border.color: control.activeFocus ? StyleColors.borderFocus : (control.enabled ? StyleColors.border : palette.dark)
    }
}
