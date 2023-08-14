import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Templates as T
import "./colors.js" as Colors
import "./sizes.js" as Sizes

T.ToolBar {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             contentHeight + topPadding + bottomPadding)

    contentWidth: contentItem.implicitWidth
                  || (contentChildren.length === 1 ? contentChildren[0].implicitWidth : 0)
    contentHeight: contentItem.implicitHeight
                   || (contentChildren.length === 1 ? contentChildren[0].implicitHeight : 0)

    background: Rectangle {
        implicitHeight: Sizes.toolbarHeight
        color: Colors.alternateBase

        Rectangle {
            color: Colors.dark
            anchors.top: control.position === ToolBar.Footer ? parent.top : undefined
            anchors.bottom: control.position === ToolBar.Header ? parent.bottom : undefined
            anchors.left: parent.left
            anchors.right: parent.right
            height: 1
        }
    }
}
