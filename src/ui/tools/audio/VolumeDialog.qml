import QtQuick 2.5
import QtQuick.Controls 2.3

import FontAwesome 2.0

Dialog {
    id: root

    property real initialMusicVolume: 1
    property real initialSoundVolume: 1

    signal musicVolumeChanged(real value)
    signal soundVolumeChanged(real value)

    modal: false

    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

    contentItem: Row {

        VolumeSlider {
            id: music
            icon: FontAwesome.music
            initialVolume: initialMusicVolume

            onVolumeChanged: musicVolumeChanged(value)
        }

        VolumeSlider {
            id: sound
            icon: FontAwesome.drum
            initialVolume: initialSoundVolume

            onVolumeChanged: soundVolumeChanged(value)
        }
    }
}
