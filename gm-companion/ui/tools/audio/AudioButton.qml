import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4

Rectangle {
    property var element_name
    property var icon_path
    property var element_type
    property var default_icon: {
        if (element_type === 0) {
            "/icons/media/music_image.png"
        } else if (element_type === 1) {
            "/icons/media/sound_image.png"
        } else {
            "/icons/media/radio_image.png"
        }
    }

    signal clicked(string element_name)

    width: 150
    height: 180

    Rectangle {
        id: rect
        color: mouse_area.pressed ? "#262f31" : "lightgrey"
        width: parent.width
        height: width

        Image {
            id: large_icon
            source: icon_path === default_icon ? icon_path : "file:///" + icon_path

            x: 5
            y: 5

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

    Text {
        y: rect.height
        x: 5
        width: parent.width - 10
        text: element_name
        color: "#262f31"
        anchors.horizontalCenter: parent.horizontalCenter
        wrapMode: Text.WordWrap
    }

    MouseArea {
        anchors.fill: parent
        id: mouse_area

        onClicked: parent.clicked(parent.element_name)
    }
}