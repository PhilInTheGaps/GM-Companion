import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2

Page {
    id: spotify_page

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    TextField {
        id: id_textfield
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: color_scheme.toolbarHeight

        selectByMouse: true
        placeholderText: qsTr("Playlist ID / Album ID")
        text: audio_editor.id

        onTextEdited: audio_editor.setId(audio_editor.name,
                                         audio_editor.type, text)
    }
}
