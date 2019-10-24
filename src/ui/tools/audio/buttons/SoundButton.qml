import QtQuick 2.9
import QtQuick.Controls 2.2

import FontAwesome 2.0

Button {
    id: root
    property var element
    property var element_icon
    property bool has_icon: true

    height: color_scheme.toolbarHeight
    hoverEnabled: true

    Image {
        id: icon
        source: if (element_icon.background.startsWith("http")
                        || element_icon.background.startsWith("data:")) {
                    element_icon.background
                } else if (has_icon) {
                    "file:///" + element_icon.background
                } else {
                    "/icons/media/sound_image.png"
                }

        anchors.fill: parent

        sourceSize.width: width
        sourceSize.height: height

        fillMode: Image.PreserveAspectCrop
    }

    Rectangle {
        id: text_overlay
        color: Qt.rgba(0, 0, 0, 0.4)
        anchors.fill: parent

        Text {
            id: text
            text: element
            color: "white"

            anchors.fill: parent
            anchors.margins: 5

            //        font.pointSize: 12
            clip: true
            elide: Text.ElideRight

            verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle {
        id: overlay
        anchors.fill: parent
        color: Qt.rgba(0, 0, 0, 0.5)
        visible: parent.hovered

        Text {
            text: FontAwesome.times
            font.pixelSize: parent.height - 10
            font.family: FontAwesome.familySolid
            anchors.centerIn: parent
            color: parent.parent.pressed ? "darkgrey" : "white"
        }
    }

    background: Rectangle {
        color: "transparent"
    }

    onClicked: {
        audio_tool.stopSound(element)
    }
}
