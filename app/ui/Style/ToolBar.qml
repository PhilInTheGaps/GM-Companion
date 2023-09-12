import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T

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
        implicitHeight: StyleSizes.toolbarHeight
        color: palette.alternateBase

        Rectangle {
            color: palette.dark
            anchors.top: control.position === ToolBar.Footer ? parent.top : undefined
            anchors.bottom: control.position === ToolBar.Header ? parent.bottom : undefined
            anchors.left: parent.left
            anchors.right: parent.right
            height: 1
        }
    }
}
