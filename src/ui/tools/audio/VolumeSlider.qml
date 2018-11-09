import QtQuick 2.5
import QtQuick.Controls 2.2

import FontAwesome 2.0

Column {
    id: root

    property string icon: FontAwesome.question
    property string iconFont: FontAwesome.familySolid
    property real initialVolume: 0

    signal volumeChanged(real value)

    spacing: 10

    Text {
        text: FontAwesome.volumeUp
        font.family: FontAwesome.familySolid
        font.pointSize: 15
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Slider {
        orientation: Qt.Vertical
        height: 200
        anchors.horizontalCenter: parent.horizontalCenter
        value: initialVolume
        from: 0
        to: 2

        onValueChanged: volumeChanged(value)
    }

    Text {
        text: FontAwesome.volumeOff
        font.family: FontAwesome.familySolid
        font.pointSize: 15
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Text {
        text: icon
        font.family: iconFont
        font.pointSize: 15
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
