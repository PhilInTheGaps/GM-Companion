import QtQuick 2.9
import QtQuick.Controls 2.2
import FontAwesome 2.0
import "./buttons"

ToolBar {
    id: audio_control_bar
    height: platform.isAndroid ? parent.height / 12 : color_scheme.toolbarHeight
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    visible: true

    background: Rectangle {
        color: color_scheme.menuColor
    }

    // Three Bars
    ControlBarButton {
        id: audio_project_structure_button
        fa_icon: FontAwesome.bars
        onClicked: left_menu.visible = !left_menu.visible
    }

    // Volume Control Button
    ControlBarButton {
        id: volume_button
        fa_icon: FontAwesome.volumeUp
        x: parent.width - width - 10 - playlist_button.width

        onClicked: volume_item.visible = !volume_item.visible
    }

    // Playlist Button
    ControlBarButton {
        id: playlist_button
        fa_icon: FontAwesome.listOl
        x: parent.width - width - 5
        onClicked: audio_info_frame.visible ? audio_info_frame.visible
                                              = false : audio_info_frame.visible = true
    }

    // Play Pause Skip
    Row {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        // Back
        ControlBarButton {
            fa_icon: FontAwesome.backward
            onClicked: audio_tool.again()
        }

        // Play Pause
        ControlBarButton {
            anchors.margins: 0
            fa_icon: audio_tool.isPaused ? FontAwesome.playCircle : FontAwesome.pauseCircle
            onClicked: audio_tool.playPause()
        }

        // Skip
        ControlBarButton {
            fa_icon: FontAwesome.forward
            onClicked: audio_tool.next()
        }
    }
}
