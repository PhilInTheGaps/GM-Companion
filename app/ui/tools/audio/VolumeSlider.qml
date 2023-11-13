import QtQuick
import QtQuick.Controls
import IconFonts

Slider {
    id: slider

    property string icon: FontAwesome.question
    property font iconFont: FontAwesome.fontSolid
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
        color: slider.pressed ? palette.text : palette.midlight
        border.color: palette.dark

        Text {
            text: slider.icon
            font.family: slider.iconFont.family
            font.styleName: slider.iconFont.styleName
            font.pixelSize: parent.height / 2
            anchors.centerIn: parent
            color: palette.alternateBase
        }
    }
}
