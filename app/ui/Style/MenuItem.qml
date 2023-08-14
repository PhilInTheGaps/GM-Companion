import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Templates as T
import "./colors.js" as Colors
import "./sizes.js" as Sizes

T.MenuItem {
    id: control

    implicitWidth: Math.max(
                       background ? background.implicitWidth : 0,
                       contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(
                        background ? background.implicitHeight : 0, Math.max(
                            contentItem.implicitHeight,
                            indicator ? indicator.implicitHeight : 0) + topPadding + bottomPadding)
    baselineOffset: contentItem.y + contentItem.baselineOffset

    padding: 6
    hoverEnabled: true

    contentItem: Label {
        leftPadding: control.checkable
                     && !control.mirrored ? control.indicator.width + control.spacing : 0
        rightPadding: control.checkable
                      && control.mirrored ? control.indicator.width + control.spacing : 0

        text: control.text
        font: control.font
        color: control.enabled ? Colors.text : Colors.textDisabled
        elide: Text.ElideRight
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: Sizes.menuItemHeight
        x: 1
        y: 1
        width: parent.width - 2
        height: parent.height - 2
        color: "transparent"
        opacity: enabled ? 1 : 0.3
        border.color: Colors.border
        border.width: control.hovered ? 1 : 0
    }
}
