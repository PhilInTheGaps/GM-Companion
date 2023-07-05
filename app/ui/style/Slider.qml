import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Templates as T
import "../colors.js" as Colors

T.Slider {
    id: control

    implicitWidth: Math.max(
                       background ? background.implicitWidth : 0,
                       (handle ? handle.implicitWidth : 0) + leftPadding + rightPadding)
    implicitHeight: Math.max(
                        background ? background.implicitHeight : 0,
                        (handle ? handle.implicitHeight : 0) + topPadding + bottomPadding)

    padding: 6

    handle: Rectangle {
        x: control.leftPadding + (horizontal ? control.visualPosition
                                               * (control.availableWidth
                                                  - width) : (control.availableWidth - width) / 2)
        y: control.topPadding + (horizontal ? (control.availableHeight - height)
                                              / 2 : control.visualPosition
                                              * (control.availableHeight - height))
        implicitWidth: 26
        implicitHeight: 26
        radius: width / 2
        color: control.enabled ? (control.pressed ? (control.visualFocus ? Colors.focus : Colors.textDisabled) : (control.visualFocus ? Colors.focus : Colors.text)) : Colors.textDisabled
        border.width: control.visualFocus ? 2 : 1
        border.color: Colors.dark

        readonly property bool horizontal: control.orientation === Qt.Horizontal
    }

    background: Rectangle {
        x: control.leftPadding + (horizontal ? 0 : (control.availableWidth - width) / 2)
        y: control.topPadding + (horizontal ? (control.availableHeight - height) / 2 : 0)
        implicitWidth: horizontal ? 200 : 6
        implicitHeight: horizontal ? 6 : 200
        width: horizontal ? control.availableWidth : implicitWidth
        height: horizontal ? implicitHeight : control.availableHeight
        color: Colors.midlight
        scale: horizontal && control.mirrored ? -1 : 1

        readonly property bool horizontal: control.orientation === Qt.Horizontal

        Rectangle {
            width: parent.horizontal ? control.visualPosition * parent.width : parent.width
            height: parent.horizontal ? parent.height : control.visualPosition * parent.height
            color: Colors.alternateBase
        }
    }
}
