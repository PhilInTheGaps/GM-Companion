import QtQuick 2.6
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0

import "./buttons"
import "../../defines.js" as Defines

ToolBar {
    id: audio_control_bar

    height: Defines.TOOLBAR_HEIGHT
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.right: parent.right

    background: Rectangle {
        color: palette.alternateBase
    }

    // Playlist Button
    CustomToolBarButton {
        id: playlist_button
        iconText: FontAwesome.listOl
        anchors.right: parent.right
        anchors.rightMargin: 10
        onClicked: info_view.visible = !info_view.visible
        pointSize: 18
    }

    // Play Pause Skip
    Row {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        // Back
        CustomToolBarButton {
            iconText: FontAwesome.backward
            onClicked: audio_tool.again()
            pointSize: 18
        }

        // Play Pause
        CustomToolBarButton {
            anchors.margins: 0
            iconText: audio_tool.isPaused ? FontAwesome.playCircle : FontAwesome.pauseCircle
            onClicked: audio_tool.playPause()
            pointSize: 24
        }

        // Skip
        CustomToolBarButton {
            iconText: FontAwesome.forward
            onClicked: audio_tool.next()
            pointSize: 18
        }
    }
}
