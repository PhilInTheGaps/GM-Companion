import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import gm.companion.colorscheme 1.0
import gm.companion.platforms 1.0

import FontAwesome 2.0

Rectangle {
    property var element_name
    property var icon_path
    property var element_type
    property var default_icon: {
        if (element_type === 0) {
            FontAwesome.music
        } else if (element_type === 1) {
            FontAwesome.drum
        } else if (element_type === 2) {
            FontAwesome.broadcastTower
        } else if (element_type === 3) {
            FontAwesome.spotify
        }
    }

    property var default_image: {
        if (element_type === 0) {
            "/icons/media/music_image.png"
        } else if (element_type === 1) {
            "/icons/media/sound_image.png"
        } else if (element_type === 2) {
            "/icons/media/radio_image.png"
        } else if (element_type === 3) {
            ""
        }
    }

    signal clicked(string element_name)

    ColorScheme {
        id: color_scheme
    }

    PlatformDetails {
        id: platform
    }

    width: {
        parent.button_width
    }

    height: width + text.height

    color: color_scheme.backgroundColor

    Rectangle {
        id: rect
        color: mouse_area.pressed ? "#262f31" : "lightgrey"
        width: parent.width
        height: width

        Image {
            id: large_icon
            source: parent ? (icon_path === default_image
                              || icon_path.startsWith(
                                  "http") ? icon_path : "file:///" + icon_path) : ""
            x: 5
            y: 5

            asynchronous: true
            width: parent.width - 10
            height: width

            sourceSize.width: width
            sourceSize.height: height
        }

        BusyIndicator {
            visible: large_icon.status == Image.Loading
            anchors.verticalCenter: large_icon.verticalCenter
            anchors.horizontalCenter: large_icon.horizontalCenter
        }

        Text {
            visible: icon_path === default_icon ? false : true
            font.pixelSize: parent.height / 5
            x: 10
            y: parent.height - 10 - height

            color: "white"
            text: default_icon
            font.family: element_type === 3 ? FontAwesome.familyBrands : FontAwesome.familySolid
        }

        Rectangle {
            id: overlay
            anchors.centerIn: parent
            width: parent.width - 10
            height: width

            visible: mouse_area.containsMouse
            color: Qt.rgba(0, 0, 0, 0.4)

            Text {
                text: FontAwesome.playCircle
                font.pointSize: 35
                font.family: FontAwesome.familyRegular
                color: mouse_area.pressed ? "silver" : "white"
                anchors.centerIn: parent
            }
        }
    }

    Text {
        id: text
        y: rect.height
        x: 5
        width: parent.width - 10
        text: element_name
        color: color_scheme.textColor
        anchors.horizontalCenter: parent.horizontalCenter
        wrapMode: Text.WordWrap
        clip: true
    }

    MouseArea {
        id: mouse_area
        anchors.fill: parent
        hoverEnabled: true

        onClicked: parent.clicked(parent.element_name)
    }
}
