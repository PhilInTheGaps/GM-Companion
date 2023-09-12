import QtQuick
import QtQuick.Controls
import IconFonts
import src

Rectangle {
    id: root

    required property Map map

    property int mapIndex: -1

    anchors.left: parent.left
    anchors.right: parent.right
    height: preview_image.status !== Image.Ready ? 100 : preview_image.height

    Image {
        id: preview_image
        anchors.left: parent.left
        anchors.right: parent.right

        visible: status == Image.Ready

        sourceSize.width: width
        sourceSize.height: height
        source: root.map.imageData

        asynchronous: true
    }

    Label {
        id: name_label
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        text: root.map.name
        wrapMode: Text.WordWrap
        elide: Text.ElideRight
        padding: 5

        background: Rectangle {
            color: "black"
            opacity: 0.7
        }
    }

    Label {
        text: FontAwesome.locationDot
        font.family: FontAwesome.fontSolid.family
        font.styleName: FontAwesome.fontSolid.styleName
        anchors.bottom: name_label.top
        anchors.right: parent.right
        anchors.margins: 5

        style: Text.Outline
        styleColor: palette.dark

        font.pixelSize: 25

        visible: root.map.hasMarkers
    }

    BusyIndicator {
        visible: preview_image.status !== Image.Ready
        anchors.centerIn: parent
    }

    MouseArea {
        anchors.fill: parent
        id: mouse_area

        onClicked: {
            MapTool.mapIndex = root.mapIndex
        }
    }
}
