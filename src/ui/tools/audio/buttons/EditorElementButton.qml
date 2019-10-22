import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import FontAwesome 2.0

AudioButton {
    overlay_enabled: false
    small_mode: true

    onClicked: {
        audio_editor.loadElement(element_name, element_type, subscenario_name)
    }
}
