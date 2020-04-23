import QtQuick 2.6
import QtQuick.Controls 2.2

import FontAwesome 2.0

Rectangle {
    id: root

    property real initialMusicVolume: 1
    property real initialSoundVolume: 1

    signal musicVolumeChanged(real value)
    signal soundVolumeChanged(real value)

    height: column.height
    color: "transparent"

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents: false
    }

    Column {
        id: column
        anchors.left: parent.left
        anchors.right: parent.right
        topPadding: 5
        bottomPadding: 5

        VolumeSlider {
            id: music
            icon: FontAwesome.music
            initialVolume: initialMusicVolume
            anchors.left: parent.left
            anchors.right: parent.right

            onValueChanged: audio_tool.setMusicVolume(value)
        }

        VolumeSlider {
            id: sound
            icon: FontAwesome.drum
            initialVolume: initialSoundVolume
            anchors.left: parent.left
            anchors.right: parent.right

            onValueChanged: audio_tool.setSoundVolume(value)
        }
    }
}
