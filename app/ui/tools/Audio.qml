pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import "./audio"
import "./audio/editor"

Item {
    id: root

    StackView {
        id: audio_stack
        anchors.fill: parent
        clip: true
        initialItem: audio_page
    }

    AudioPage {
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

        sourceComponent: Editor {
            onBackToTool: {
                audio_stack.pop()
                audio_editor_page.active = false
            }
        }
    }
}
