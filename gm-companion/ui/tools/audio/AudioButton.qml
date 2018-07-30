import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import gm.companion.colorscheme 1.0
import gm.companion.platforms 1.0
import "../../fontawesome"

Rectangle {
    property var element_name
    property var icon_path
    property var element_type
    property var default_icon: {
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

    width: platform.isAndroid ? (parent ? parent.parent.width / 8 : 0) : 150
    height: platform.isAndroid ? width + text.height : 180

    color: color_scheme.backgroundColor

    Rectangle {
        id: rect
        color: mouse_area.pressed ? "#262f31" : "lightgrey"
        width: parent.width
        height: width

        Image {
            id: large_icon
            source: icon_path === default_icon || icon_path.startsWith(
                        "http") ? icon_path : "file:///" + icon_path
            x: 5
            y: 5

            asynchronous: true
            width: parent.width - 10
            height: width

            sourceSize.width: width
            sourceSize.height: height
        }

        Image {
            id: small_icon

            visible: icon_path === default_icon ? false : true
            source: default_icon

            width: parent.width / 5
            height: width

            x: 10
            y: parent.height - 10 - width

            sourceSize.width: width
            sourceSize.height: height
        }
    }

    Icon {
        visible: element_type === 3

        font.pixelSize: parent.width / 5
        x: small_icon.x
        y: small_icon.y

        color: "white"
        icon: icons.fab_spotify
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
        anchors.fill: parent
        id: mouse_area

        onClicked: parent.clicked(parent.element_name)
    }
}
