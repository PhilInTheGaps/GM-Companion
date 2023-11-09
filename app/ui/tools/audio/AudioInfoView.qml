pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import src

Item {
    id: audio_info_frame

    Column {
        id: top_item
        anchors.margins: 10
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        // Meta Data
        MetaDataDisplay {
            id: meta_data_display
            anchors.left: parent.left
            anchors.right: parent.right
        }

        // Spacer
        Item {
            width: 5
            height: 15
        }

        // "Playlist" label
        Label {
            id: playlist_menu_text
            anchors.left: parent.left
            anchors.right: parent.right

            text: qsTr("Playlist")
            wrapMode: Text.WordWrap
            font.bold: true
            visible: playlist_view.count > 0
        }
    }

    // Playlist
    ListView {
        id: playlist_view
        anchors.top: top_item.bottom
        anchors.bottom: bottom_item.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5

        clip: true

        highlight: Rectangle {
            color: palette.highlight
        }

        ScrollBar.vertical: ScrollBar {
            anchors.right: playlist_view.right
            visible: playlist_view.contentHeight > playlist_view.height
        }

        model: AudioTool.playlist
        currentIndex: AudioTool.index
        highlightMoveVelocity: -1

        delegate: Item {
            id: playlist_delegate

            // TODO: type
            required property AudioFile modelData
            required property int index

            width: playlist_view.width
            height: playlist_text.height + 10

            Text {
                id: playlist_text
                clip: true
                elide: Text.ElideRight
                text: playlist_delegate.modelData.displayName
                color: playlist_view.currentIndex === playlist_delegate.index ? palette.highlightedText : palette.text
                font.pointSize: 10
                anchors.centerIn: parent
                width: parent.width - 10
            }

            ToolTip {
                id: playlist_tooltip
                text: playlist_text.text + " [" + playlist_delegate.modelData.url + "]"
            }

            MouseArea {
                hoverEnabled: true
                anchors.fill: parent

                onClicked: AudioTool.setMusicIndex(playlist_delegate.index)
                onEntered: playlist_tooltip.visible = true
                onExited: playlist_tooltip.visible = false
            }
        }
    }

    Column {
        id: bottom_item
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        // Volume Dialog
        VolumeItem {
            id: volume_item
            anchors.left: parent.left
            anchors.right: parent.right

            visible: true

            initialMusicVolume: AudioTool.musicVolume
            initialSoundVolume: AudioTool.soundVolume
        }
    }
}
