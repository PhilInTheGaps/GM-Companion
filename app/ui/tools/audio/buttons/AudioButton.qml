import QtQuick 2.6
import QtQuick.Controls 2.2

import FontAwesome 2.0
import "../../../defines.js" as Defines

Rectangle {
    id: root
    property string element_name
    property var icon
    property int element_type
    property bool overlay_enabled: true
    property bool small_mode: false
    property string subscenario_name: ""

    signal clicked

    property var type_icon: {
        if (element_type === 0) {
            FontAwesome.music
        } else if (element_type === 1) {
            FontAwesome.drum
        } else if (element_type === 2) {
            FontAwesome.broadcastTower
        }
    }

    height: small_mode ? Defines.TOOLBAR_HEIGHT : width

    Image {
        id: thumbnail

        source: "image://audioElementIcons/" + icon.imageId
        anchors.fill: parent
        asynchronous: true
        cache: false

        fillMode: small_mode ? Image.PreserveAspectCrop : Image.Stretch

        sourceSize.width: 400
        sourceSize.height: 400
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
        height: small_mode ? parent.height : parent.width / 4

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
