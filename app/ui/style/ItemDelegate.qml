import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Templates as T
import "../colors.js" as Colors
import "../sizes.js" as Sizes

T.ItemDelegate {
    id: control

    implicitWidth: Math.max(
                       background ? background.implicitWidth : 0,
                       contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(
                        background ? background.implicitHeight : 0, Math.max(
                            contentItem.implicitHeight,
                            indicator ? indicator.implicitHeight : 0) + topPadding + bottomPadding)
    baselineOffset: contentItem.y + contentItem.baselineOffset

    padding: 12
    spacing: 12

    hoverEnabled: true
    font.pointSize: 12

    contentItem: Label {
        leftPadding: control.mirrored ? (control.indicator ? control.indicator.width : 0)
                                        + control.spacing : 0
        rightPadding: !control.mirrored ? (control.indicator ? control.indicator.width : 0)
                                          + control.spacing : 0

        text: control.text
        font: control.font
        color: control.enabled ? Colors.text : Colors.textDisabled
        opacity: control.pressed ? 0.7 : 1
        elide: Text.ElideRight
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: Sizes.delegateHeight
        visible: control.down || control.highlighted || control.visualFocus
        color: Colors.dark
        border.color: Colors.border
        border.width: hovered ? 1 : 0
    }
}
