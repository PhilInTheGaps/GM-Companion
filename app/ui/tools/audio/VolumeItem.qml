import QtQuick
import IconFonts
import src

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
            initialVolume: root.initialMusicVolume
            anchors.left: parent.left
            anchors.right: parent.right

            onValueChanged: {
                AudioTool.setMusicVolume(value)
            }
        }

        VolumeSlider {
            id: sound
            icon: FontAwesome.drum
            initialVolume: root.initialSoundVolume
            anchors.left: parent.left
            anchors.right: parent.right

            onValueChanged: {
                AudioTool.setSoundVolume(value)
            }
        }
    }
}
