import QtQuick
import QtQuick.Templates as T

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
        color: control.checked
               || control.highlighted ? StyleColors.focus : (control.visualFocus ? StyleColors.focus : (control.down ? StyleColors.textDisabled : palette.text))
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 40
        visible: !control.flat || control.down || control.checked
                 || control.highlighted
        color: palette.dark
        border.color: palette.button
        border.width: control.visualFocus || control.hovered ? 1 : 0
    }
}
