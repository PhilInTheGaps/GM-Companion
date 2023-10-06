import QtQuick
import CustomComponents
import src

Column {
    spacing: 10

    CustomScrollLabel {
        id: music_element_name
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 5
        text: AudioTool.currentElementName
        font.bold: true
    }

    CustomScrollLabel {
        id: song_name_text
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 5
        text: AudioTool.metaData.title
    }

    CustomScrollLabel {
        id: artist_text
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 5
        text: AudioTool.metaData.artist.join(", ")
    }

    CustomScrollLabel {
        id: album_text
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 5
        text: AudioTool.metaData.album
    }
}
