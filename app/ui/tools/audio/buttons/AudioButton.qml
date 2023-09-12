import QtQuick
import QtQuick.Controls
import IconFonts
import src
import "../../.."

Rectangle {
    id: root
    property string element_name
    property AudioThumbnail thumbnail
    property int element_type
    property bool overlay_enabled: true
    property bool small_mode: false
    property string subscenario_name: ""
    property bool hover_enabled: true

    signal clicked

    property string type_icon: {
        if (element_type === 0) {
            FontAwesome.music
        } else if (element_type === 1) {
            FontAwesome.drum
        } else if (element_type === 2) {
            FontAwesome.towerBroadcast
        }
    }

    color: palette.dark

    height: small_mode ? Sizes.toolbarHeight : width

    Image {
        id: thumbnail_image

        source: root.thumbnail ? "image://audioElementIcons/" + root.thumbnail.imageId : ""
        anchors.fill: parent
        asynchronous: true
        cache: false

        fillMode: root.small_mode ? Image.PreserveAspectCrop : Image.Stretch

        sourceSize.width: 400
        sourceSize.height: 400
    }

    BusyIndicator {
        visible: thumbnail_image.status == Image.Loading
        anchors.verticalCenter: thumbnail_image.verticalCenter
        anchors.horizontalCenter: thumbnail_image.horizontalCenter
    }

    Rectangle {
        id: overlay
        anchors.fill: parent

        visible: mouse_area.containsMouse
        color: Qt.rgba(0, 0, 0, 0.4)

        Text {
            text: FontAwesome.circlePlay
            font.pointSize: 35
            font.family: FontAwesome.fontRegular.family
            font.styleName: FontAwesome.fontRegular.styleName
            color: mouse_area.pressed ? "silver" : "white"
            anchors.centerIn: parent
            visible: root.overlay_enabled
        }
    }

    Rectangle {
        id: text_rect

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: root.small_mode ? parent.height : parent.width / 4

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
            text: root.element_name
            color: "white"
        }

        Text {
            id: type_icon_text
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: 10

            font.pixelSize: parent.height * 0.5
            color: "white"
            text: root.type_icon
            font.family: root.element_type
                         === 3 ? FontAwesome.fontBrands.family : FontAwesome.fontSolid.family
            font.styleName: root.element_type === 3 ? FontAwesome.fontBrands.styleName : FontAwesome.fontSolid.styleName
        }
    }

    MouseArea {
        id: mouse_area
        anchors.fill: parent
        hoverEnabled: root.hover_enabled

        onClicked: root.clicked()
    }
}
