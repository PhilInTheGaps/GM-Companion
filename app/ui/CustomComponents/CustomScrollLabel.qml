import QtQuick 2.9
import QtQuick.Controls 2.2

Item {
    id: root
    height: label.height
    clip: true

    property alias text: label.text
    property alias font: label.font
    property alias padding: label.padding

    Label {
        id: label

        property int scrollLength: (label.implicitWidth - root.width)

        elide: Text.ElideRight
        width: animation.running ? implicitWidth : parent.width

        SequentialAnimation on x {
            id: animation
            alwaysRunToEnd: true
            running: false

            NumberAnimation { to: -label.scrollLength ; duration: label.implicitWidth * 10; easing.type: Easing.Linear}
            PauseAnimation { duration: 1000 }
            NumberAnimation { to: 0 ; duration: 2000 ; easing.type: Easing.OutSine}
            PauseAnimation { duration: 1000 }
        }
    }

    MouseArea {
        id: area
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            if (label.implicitWidth > root.width) {
                animation.start()
            }
        }
    }
}
