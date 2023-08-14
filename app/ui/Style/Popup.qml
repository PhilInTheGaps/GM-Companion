import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Templates as T
import "./colors.js" as Colors

T.Popup {
    id: control

    implicitWidth: Math.max(
                       background ? background.implicitWidth : 0,
                       contentWidth > 0 ? contentWidth + leftPadding + rightPadding : 0)
    implicitHeight: Math.max(
                        background ? background.implicitHeight : 0,
                        contentHeight > 0 ? contentHeight + topPadding + bottomPadding : 0)

    contentWidth: contentItem.implicitWidth
                  || (contentChildren.length === 1 ? contentChildren[0].implicitWidth : 0)
    contentHeight: contentItem.implicitHeight
                   || (contentChildren.length === 1 ? contentChildren[0].implicitHeight : 0)

    padding: 12

    background: Rectangle {
        color: Colors.dark
        border.color: Colors.border
    }
}
