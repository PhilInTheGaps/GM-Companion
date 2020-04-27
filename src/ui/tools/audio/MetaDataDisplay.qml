import QtQuick 2.6
import QtQuick.Controls 2.2
import CustomComponents 1.0

Column {
    spacing: 10

    CustomScrollLabel {
        id: music_element_name
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 5
        text: audio_tool.type
        font.bold: true
    }

    CustomScrollLabel {
        id: song_name_text
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 5
        text: audio_tool.title
    }

    CustomScrollLabel {
        id: artist_text
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 5
        text: audio_tool.artist
    }

    CustomScrollLabel {
        id: album_text
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 5
        text: audio_tool.album
    }
}
