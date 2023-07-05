import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Templates as T
import "../colors.js" as Colors
import "../sizes.js" as Sizes

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

    color: enabled ? Colors.text : Colors.textDisabled
    selectionColor: Colors.highlight
    selectedTextColor: Colors.highlightedText
    verticalAlignment: TextInput.AlignVCenter

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

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: Sizes.textFieldHeight
        border.width: control.activeFocus ? 2 : 1
        color: Colors.window
        border.color: control.activeFocus ? Colors.borderFocus : (control.enabled ? Colors.border : Colors.dark)
    }
}
