import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import gm.companion.platforms 1.0

Button {
    property string tool_name
    property string icon_source

    id: button
    anchors.left: parent.left
    anchors.right: parent.right
    height: platform.isAndroid ? drawer.height / 15 : 45

    background: Rectangle {
        implicitHeight: 40
        color: parent.down ? "white" : "transparent"
    }

    PlatformDetails {
        id: platform
    }

    Row {
        anchors.fill: parent
        padding: 5
        spacing: 5

        Image {
            id: button_icon
            fillMode: Image.PreserveAspectFit
            anchors.verticalCenter: parent.verticalCenter
            sourceSize.height: parent.height - parent.padding * 2
            height: sourceSize.height
            source: icon_source
        }

        Text {
            text: tool_name
            font.pixelSize: platform.isAndroid ? parent.height / 2 : 17
            visible: !inPortrait || !altMenu

            width: parent.width - parent.spacing - parent.padding * 2 - button_icon.width
            clip: true
            elide: Text.ElideRight

            opacity: enabled ? 1.0 : 0.3
            color: button.down ? "black" : "white"

            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
