import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Templates as T
import "../colors.js" as Colors

T.Frame {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             contentHeight + topPadding + bottomPadding)

    contentWidth: contentItem.implicitWidth
                  || (contentChildren.length === 1 ? contentChildren[0].implicitWidth : 0)
    contentHeight: contentItem.implicitHeight
                   || (contentChildren.length === 1 ? contentChildren[0].implicitHeight : 0)

    padding: 12

    background: Rectangle {
        color: "transparent"
        border.color: Colors.border
    }
}
