import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0

Item {
    id: audio_info_frame

    // Meta Data
    MetaDataDisplay {
        id: meta_data_display
        anchors.topMargin: 15
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }

    Label {
        id: playlist_menu_text
        anchors.top: meta_data_display.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 15

        text: qsTr("Playlist")
        wrapMode: Text.WordWrap
        font.bold: true
        visible: playlist_view.count > 0
    }

    // Playlist
    ListView {
        id: playlist_view
        anchors.top: playlist_menu_text.bottom
        anchors.bottom: volume_item.visible ? volume_item.top : parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 5
        anchors.bottomMargin: volume_item.visible ? 5 : 0
        anchors.rightMargin: 5

        clip: true

        highlight: Rectangle {
            color: palette.highlight
        }

        ScrollBar.vertical: CustomScrollBar {
            anchors.right: playlist_view.right
            visible: playlist_view.contentHeight > playlist_view.height
        }

        model: audio_tool.songs
        currentIndex: audio_tool.index
        highlightMoveVelocity: -1

        delegate: Item {
            width: parent.width
            height: playlist_text.height + 10

            Text {
                id: playlist_text
                clip: true
                elide: Text.ElideRight
                text: modelData
                color: playlist_view.currentIndex === index ? palette.highlightedText : palette.text
                font.pointSize: 10
                anchors.centerIn: parent
                width: parent.width - 10
            }

            ToolTip {
                id: playlist_tooltip
                text: modelData
            }

            MouseArea {
                hoverEnabled: true
                anchors.fill: parent

                onClicked: audio_tool.setMusicIndex(index)
                onEntered: playlist_tooltip.visible = true
                onExited: playlist_tooltip.visible = false
            }
        }
    }

    // Volume Dialog
    VolumeItem {
        id: volume_item
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        visible: true

        initialMusicVolume: audio_tool.musicVolume
        initialSoundVolume: audio_tool.soundVolume
    }
}
