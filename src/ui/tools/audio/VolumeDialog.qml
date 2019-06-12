import QtQuick 2.5
import QtQuick.Controls 2.3

import FontAwesome 2.0

Rectangle {
    id: root

    property real initialMusicVolume: 1
    property real initialSoundVolume: 1

    signal musicVolumeChanged(real value)
    signal soundVolumeChanged(real value)

    width: row.width + 20
    height: row.height + 20

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents: false
    }

    Row {
        id: row
        anchors.centerIn: parent

        VolumeSlider {
            id: music
            icon: FontAwesome.music
            initialVolume: initialMusicVolume

            onVolumeChanged: audio_tool.setMusicVolume(value)
        }

        VolumeSlider {
            id: sound
            icon: FontAwesome.drum
            initialVolume: initialSoundVolume

            onVolumeChanged: audio_tool.setSoundVolume(value)
        }
    }
}
