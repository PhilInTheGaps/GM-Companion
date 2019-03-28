import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import FontAwesome 2.0

AudioButton {
    overlay_enabled: false
    small_mode: true

    onClicked: {
        switch (element_type) {
        case 0:
            // Music
            element_stack_view.replace(audio_list_page)
            break
        case 1:
            // Sound
            element_stack_view.replace(audio_list_page)
            break
        case 2:
            // Radio
            element_stack_view.replace(radio_page)
            break
        case 3:
            // Spotify
            element_stack_view.replace(spotify_page)
            break
        default:
            // No element selected
            element_stack_view.replace(no_element_text)
            return
        }

        audio_editor.loadElement(element_name, element_type)
        element_icon_field.text = audio_editor.icon
    }
}
