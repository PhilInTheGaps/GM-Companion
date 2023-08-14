import QtQuick
import QtQuick.Templates as T
import "./colors.js" as Colors

T.MenuSeparator {
    id: control

    implicitWidth: Math.max(
                       background ? background.implicitWidth : 0,
                       contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(
                        background ? background.implicitHeight : 0,
                        contentItem.implicitHeight + topPadding + bottomPadding)

    topPadding: 6
    bottomPadding: 6

    contentItem: Rectangle {
        implicitWidth: 200
        implicitHeight: 1
        color: Colors.menuSeparator
    }
}
