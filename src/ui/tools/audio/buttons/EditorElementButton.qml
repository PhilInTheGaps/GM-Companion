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

        subscenario_combo_box_rect.visible = true
        element_info_row_1.visible = true

        subscenario_combo_box.isSetEnabled = false

        audio_editor.loadElement(element_name, element_type, subscenario_name)
        element_icon_field.text = audio_editor.icon

        subscenario_combo_box.currentIndex = audio_editor.getSubscenarioIndex(
                    subscenario_name) + 1

        subscenario_combo_box.isSetEnabled = true
    }
}
