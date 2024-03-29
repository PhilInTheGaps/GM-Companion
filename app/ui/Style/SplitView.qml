pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls
import QtQuick.Controls.impl

T.SplitView {
    id: control
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    hoverEnabled: true

    handle: Rectangle {
        implicitWidth: control.orientation === Qt.Horizontal ? 1 : control.width
        implicitHeight: control.orientation === Qt.Horizontal ? control.height : 1

        color: T.SplitHandle.pressed ? StyleColors.splitViewHandlePressed : (T.SplitHandle.hovered ? StyleColors.splitViewHandleHighlight : StyleColors.splitViewHandle)

        containmentMask: Item {
            x: -width / 2
            width: 10
            height: control.height
        }
    }
}
