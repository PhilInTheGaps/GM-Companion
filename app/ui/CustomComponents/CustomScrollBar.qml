import QtQuick 2.9
import QtQuick.Controls 2.2

ScrollBar {
    id: root
    interactive: true

    contentItem: Rectangle {
        implicitWidth: 6
        radius: width / 2
        color: root.pressed ? palette.dark : palette.button
    }
}
