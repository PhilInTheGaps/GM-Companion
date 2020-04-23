import QtQuick 2.9
import QtQuick.Controls 2.2

Label {
    z: 150
    y: 20

    font.pointSize: 15
    padding: 10

    visible: false

    background: Rectangle {
        color: palette.dark
        opacity: 0.8
    }
}
