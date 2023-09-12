pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Templates as T

T.PageIndicator {
    id: control

    implicitWidth: Math.max(
                       background ? background.implicitWidth : 0,
                       contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(
                        background ? background.implicitHeight : 0,
                        contentItem.implicitHeight + topPadding + bottomPadding)

    padding: 6
    spacing: 6

    delegate: Rectangle {
        required property int index

        implicitWidth: 8
        implicitHeight: 8

        radius: width / 2
        color: StyleColors.pageIndicator

        // qmllint disable unqualified
        opacity: index === control.currentIndex ? 0.95 : pressed ? 0.7 : 0.45

        Behavior on opacity {
            OpacityAnimator {
                duration: 100
            }
        }
    }

    contentItem: Row {
        spacing: control.spacing

        Repeater {
            model: control.count
            delegate: control.delegate
        }
    }
}
