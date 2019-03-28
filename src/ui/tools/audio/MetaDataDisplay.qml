import QtQuick 2.3
import QtQuick.Controls 2.2

Column {
    spacing: 10

    Text {
        id: music_element_name
        anchors.left: parent.left
        anchors.right: parent.right

        text: audio_tool.type
        wrapMode: Text.WordWrap
        color: color_scheme.textColor
        font.bold: true
    }

    Text {
        id: song_name_text
        anchors.left: parent.left
        anchors.right: parent.right

        text: audio_tool.title
        clip: true
        elide: Text.ElideRight
        color: color_scheme.textColor
    }

    Text {
        id: artist_text
        anchors.left: parent.left
        anchors.right: parent.right

        text: audio_tool.artist
        clip: true
        elide: Text.ElideRight
        color: color_scheme.textColor
    }

    Text {
        id: album_text
        anchors.left: parent.left
        anchors.right: parent.right

        text: audio_tool.album
        clip: true
        elide: Text.ElideRight
        color: color_scheme.textColor
    }

    Image {
        id: cover_image
        anchors.left: parent.left
        anchors.right: parent.right

        visible: source != ""
        sourceSize.width: width
        source: audio_tool.cover
    }
}
