import QtQuick 2.6
import QtQuick.Controls 2.2

import FontAwesome 2.0

Slider {
    id: slider

    property string icon: FontAwesome.question
    property string iconFont: FontAwesome.familySolid
    property real initialVolume: 0

    orientation: Qt.Horizontal
    value: initialVolume
    from: 0
    to: 1
    live: false

    handle: Rectangle {
        x: slider.leftPadding + slider.visualPosition * (slider.availableWidth - width)
        y: slider.topPadding + slider.availableHeight / 2 - height / 2
        implicitWidth: 26
        implicitHeight: 26
        radius: 13
        color: slider.pressed ? palette.light : palette.midlight
        border.color: palette.dark

        Text {
            text: icon
            font.family: iconFont
            font.pixelSize: parent.height / 2
            anchors.centerIn: parent
            color: palette.alternateBase
        }
    }
}
