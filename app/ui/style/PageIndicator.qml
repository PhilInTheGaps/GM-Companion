import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.impl 2.2
import QtQuick.Templates 2.2 as T
import "../colors.js" as Colors

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
        implicitWidth: 8
        implicitHeight: 8

        radius: width / 2
        color: Colors.pageIndicator

        opacity: index === currentIndex ? 0.95 : pressed ? 0.7 : 0.45

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
