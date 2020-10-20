import QtQuick 2.6
import QtQuick.Controls 2.2

import "../main"
import "./audio"
import "./audio/editor"

Item {
    StackView {
        id: audio_stack
        anchors.fill: parent
        clip: true
        initialItem: audio_page
    }

    AudioTool {
        id: audio_page

        onOpenEditor: {
            audio_editor_page.active = true
            audio_stack.push(audio_editor_page)
        }
    }

    Loader {
        id: audio_editor_page
        active: false
        asynchronous: true
        source: "audio/editor/Editor.qml"
    }

    Connections {
        target: audio_editor_page.item

        function onBackToTool() {
            audio_stack.pop()
            audio_editor_page.active = false
        }
    }
}
