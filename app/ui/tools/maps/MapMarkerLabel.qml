import QtQuick
import QtQuick.Controls

Label {
    z: 150
    y: 20

    font.pointSize: 15
    padding: 10

    visible: false

    background: Rectangle {
        color: palette.dark
        border.color: palette.dark
        border.width: 1

        opacity: 0.8
    }
}
