import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import FontAwesome 2.0

Rectangle {
    id: root
    property var element_name
    property var icon_path
    property var element_type
    property bool has_icon: true
    property bool overlay_enabled: true
    property bool small_mode: false

    signal clicked

    property var type_icon: {
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

    property var default_icon: {
        if (element_type === 0) {
            "/icons/media/music_image.png"
        } else if (element_type === 1) {
            "/icons/media/sound_image.png"
        } else if (element_type === 2) {
            "/icons/media/radio_image.png"
        } else if (element_type === 3) {
            "/icons/media/music_image.png"
        }
    }

    height: small_mode ? width / 4 : width

    color: color_scheme.backgroundColor

    Image {
        id: thumbnail
        source: {
            if (icon_path.startsWith("http")) {
                icon_path
            } else if (has_icon) {
                "file:///" + icon_path
            } else {
                default_icon
            }
        }

        onStatusChanged: {
            if (status == Image.Error) {
                source = default_icon
            }
        }

        anchors.fill: parent
        asynchronous: true

        fillMode: small_mode ? Image.PreserveAspectCrop : Image.Stretch

        sourceSize.width: width
        sourceSize.height: height
    }

    BusyIndicator {
        visible: thumbnail.status == Image.Loading
        anchors.verticalCenter: thumbnail.verticalCenter
        anchors.horizontalCenter: thumbnail.horizontalCenter
    }

    Rectangle {
        id: overlay
        anchors.fill: parent

        visible: mouse_area.containsMouse
        color: Qt.rgba(0, 0, 0, 0.4)

        Text {
            text: FontAwesome.playCircle
            font.pointSize: 35
            font.family: FontAwesome.familyRegular
            color: mouse_area.pressed ? "silver" : "white"
            anchors.centerIn: parent
            visible: overlay_enabled
        }
    }

    Rectangle {
        id: text_rect

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: parent.width / 4

        color: Qt.rgba(0, 0, 0, 0.5)

        Text {
            id: text

            anchors.left: parent.left
            anchors.right: type_icon_text.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 5

            clip: true
            elide: Text.ElideRight
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
            text: element_name
            color: "white"
        }

        Text {
            id: type_icon_text
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: 10

            font.pixelSize: parent.height * 0.5
            color: "white"
            text: type_icon
            font.family: element_type === 3 ? FontAwesome.familyBrands : FontAwesome.familySolid
        }
    }

    MouseArea {
        id: mouse_area
        anchors.fill: parent
        hoverEnabled: true

        onClicked: root.clicked()
    }
}
