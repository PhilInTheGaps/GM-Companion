import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Templates as T
import "../colors.js" as Colors

T.Button {
    id: control

    implicitWidth: Math.max(
                       background ? background.implicitWidth : 0,
                       contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(
                        background ? background.implicitHeight : 0,
                        contentItem.implicitHeight + topPadding + bottomPadding)
    baselineOffset: contentItem.y + contentItem.baselineOffset

    padding: 6
    leftPadding: padding + 2
    rightPadding: padding + 2
    hoverEnabled: true

    font.bold: control.hovered

    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled || control.highlighted || control.checked ? 1 : 0.3
        //        color: control.checked
        //               || control.highlighted ? Colors.light : (control.visualFocus ? Colors.text : (control.down ? Colors.mid : Colors.buttonText))
        color: control.checked
               || control.highlighted ? Colors.focus : (control.visualFocus ? Colors.focus : (control.down ? Colors.textDisabled : Colors.text))
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 40
        visible: !control.flat || control.down || control.checked
                 || control.highlighted
        //        color: control.checked || control.highlighted ?
        //            (control.visualFocus ? (control.down ? Default.buttonCheckedFocusColor : Default.focusColor) : (control.down ? Default.buttonCheckedPressedColor : Default.textColor)) :
        //            (control.visualFocus ? (control.down ? Default.focusPressedColor : Default.focusLightColor) : (control.down ? Default.buttonPressedColor : Default.buttonColor))
        color: Colors.dark
        border.color: Colors.button
        border.width: control.visualFocus || control.hovered ? 1 : 0
    }
}
