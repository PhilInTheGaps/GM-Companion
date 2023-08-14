import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls
import QtQuick.Controls.impl
import "./colors.js" as Colors

T.SplitView {
    id: control
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    hoverEnabled: true

    handle: Rectangle {
        implicitWidth: control.orientation === Qt.Horizontal ? 5 : control.width
        implicitHeight: control.orientation === Qt.Horizontal ? control.height : 5
        color: T.SplitHandle.pressed ? Colors.splitViewHandlePressed : (T.SplitHandle.hovered ? Colors.splitViewHandleHighlight : Colors.splitViewHandle)
    }
}
