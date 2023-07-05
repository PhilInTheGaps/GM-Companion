import QtQuick
import QtQuick.Templates as T
import "../colors.js" as Colors

T.DialogButtonBox {
    id: control

    implicitWidth: Math.max(
                       background ? background.implicitWidth : 0,
                       contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(
                        background ? background.implicitHeight : 0,
                        contentItem.implicitHeight + topPadding + bottomPadding)

    spacing: 5
    padding: 12
    alignment: Qt.AlignRight

    delegate: Button {}

    contentItem: ListView {
        implicitWidth: contentWidth
        implicitHeight: 40

        model: control.contentModel
        spacing: control.spacing
        orientation: ListView.Horizontal
        boundsBehavior: Flickable.StopAtBounds
        snapMode: ListView.SnapToItem
    }

    background: Rectangle {
        implicitHeight: 40
        x: 1
        y: 1
        width: parent.width - 2
        height: parent.height - 2

        color: Colors.window
    }
}
