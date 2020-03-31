import QtQuick 2.9
import QtQuick.Controls 2.2
import FontAwesome 2.0

Rectangle {
    property var name
    property var path
    property var imageData: ""
    property var markers
    property bool hasMarkers: false
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
        source: imageData

        asynchronous: true
    }

    Label {
        id: name_label
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        text: parent.name
        color: "white"
        wrapMode: Text.WordWrap
        elide: Text.ElideRight
        padding: 5

        background: Rectangle {
            color: "black"
            opacity: 0.7
        }
    }

    Text {
        text: FontAwesome.mapMarkerAlt
        font.family: FontAwesome.familySolid
        color: "white"
        anchors.bottom: name_label.top
        anchors.right: parent.right
        anchors.margins: 5

        style: Text.Outline
        styleColor: "black"

        font.pixelSize: 25

        visible: hasMarkers
    }

    BusyIndicator {
        visible: preview_image.status !== Image.Ready
        anchors.centerIn: parent
    }

    MouseArea {
        anchors.fill: parent
        id: mouse_area

        onClicked: {
            map_tool.mapIndex = mapIndex
            image.source = imageData
            image.rotation = 0

            if (hasMarkers) {
                left_swipe_view.currentIndex = 1
            }
        }
    }
}
