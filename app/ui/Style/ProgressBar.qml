import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls
import QtQuick.Controls.impl
import "./sizes.js" as Sizes
import "./colors.js" as Colors

T.ProgressBar {
    id: control

    implicitWidth: Math.max(
                       background ? background.implicitWidth : 0,
                       contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(
                        background ? background.implicitHeight : 0,
                        contentItem.implicitHeight + topPadding + bottomPadding)

    contentItem: Item {
        implicitHeight: Sizes.progressBarHeight
        implicitWidth: 116

        Rectangle {
            scale: control.mirrored ? -1 : 1

            width: control.visualPosition * parent.width
            height: parent.height

            radius: 2
            color: Colors.alternateBase
        }
    }

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: Sizes.progressBarHeight
        x: control.leftPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        width: control.availableWidth
        height: Sizes.progressBarHeight

        color: Colors.window
        border.color: Colors.border
        border.width: 1
    }
}
