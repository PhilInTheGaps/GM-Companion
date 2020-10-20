import QtQuick 2.13
import QtQuick.Templates 2.13 as T
import QtQuick.Controls 2.13
import QtQuick.Controls.impl 2.13
import "../colors.js" as Colors

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
