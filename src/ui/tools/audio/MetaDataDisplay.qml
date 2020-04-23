import QtQuick 2.6
import QtQuick.Controls 2.2

Column {
    spacing: 10

    Label {
        id: music_element_name
        anchors.left: parent.left
        anchors.right: parent.right

        text: audio_tool.type
        wrapMode: Text.WordWrap
        font.bold: true
    }

    Label {
        id: song_name_text
        anchors.left: parent.left
        anchors.right: parent.right

        text: audio_tool.title
        clip: true
        elide: Text.ElideRight
    }

    Label {
        id: artist_text
        anchors.left: parent.left
        anchors.right: parent.right

        text: audio_tool.artist
        clip: true
        elide: Text.ElideRight
    }

    Label {
        id: album_text
        anchors.left: parent.left
        anchors.right: parent.right

        text: audio_tool.album
        clip: true
        elide: Text.ElideRight
    }
}
