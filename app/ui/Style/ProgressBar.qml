import QtQuick
import QtQuick.Templates as T

T.ProgressBar {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0, contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0, contentItem.implicitHeight + topPadding + bottomPadding)

    contentItem: Item {
        implicitHeight: StyleSizes.progressBarHeight
        implicitWidth: 116

        Rectangle {
            scale: control.mirrored ? -1 : 1

            width: control.visualPosition * parent.width
            height: parent.height

            radius: 2
            color: palette.alternateBase
        }
    }

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: StyleSizes.progressBarHeight
        x: control.leftPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        width: control.availableWidth
        height: StyleSizes.progressBarHeight

        color: palette.window
        border.color: palette.button
        border.width: 1
    }
}
