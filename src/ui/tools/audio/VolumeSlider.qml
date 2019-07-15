import QtQuick 2.5
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
        color: slider.pressed ? "#f0f0f0" : "#f6f6f6"
        border.color: "#bdbebf"

        Text {
            text: icon
            font.family: iconFont
            font.pixelSize: parent.height / 2
            anchors.centerIn: parent
            color: color_scheme.menuColor
        }
    }

    background: Rectangle {
        x: slider.leftPadding
        y: slider.topPadding + slider.availableHeight / 2 - height / 2
        implicitWidth: 200
        implicitHeight: 6
        width: slider.availableWidth
        height: implicitHeight
        radius: 2
        color: "#bdbebf"

        Rectangle {
            width: slider.visualPosition * parent.width
            height: parent.height
            color: color_scheme.listHeaderBackgroundColor //"#21be2b"
            radius: 2
        }
    }
}
