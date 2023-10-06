import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import src
import "../.."

ToolBar {
    id: root

    signal togglePlaylist

    height: Sizes.toolbarHeight
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.right: parent.right

    // Playlist Button
    CustomToolBarButton {
        id: playlist_button
        iconText: FontAwesome.listOl
        anchors.right: parent.right
        anchors.rightMargin: 10
        onClicked: root.togglePlaylist()
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
            onClicked: AudioTool.again()
            pointSize: 18
        }

        // Play Pause
        CustomToolBarButton {
            anchors.margins: 0
            iconText: switch(AudioTool.playbackState){
                          case AudioPlayer.Playing:
                              return FontAwesome.circlePause
                          case AudioPlayer.Loading:
                              return FontAwesome.spinner
                          default:
                              return FontAwesome.circlePlay
                      }

            onClicked: AudioTool.playPause()
            pointSize: 24
        }

        // Skip
        CustomToolBarButton {
            iconText: FontAwesome.forward
            onClicked: AudioTool.next()
            pointSize: 18
        }
    }
}
