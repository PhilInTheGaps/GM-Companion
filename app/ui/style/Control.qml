import QtQuick
import QtQuick.Templates as T
import "../colors.js" as Colors

T.Control {
    id: control

    implicitWidth: Math.max(
                       background ? background.implicitWidth : 0,
                       (contentItem ? contentItem.implicitWidth : 0) + leftPadding + rightPadding)
    implicitHeight: Math.max(
                        background ? background.implicitHeight : 0,
                        (contentItem ? contentItem.implicitHeight : 0) + topPadding + bottomPadding)

    background: Rectangle {
        color: Colors.window
    }
}
