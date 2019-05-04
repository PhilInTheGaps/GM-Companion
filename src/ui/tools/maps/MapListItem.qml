import QtQuick 2.9
import QtQuick.Controls 2.2

Rectangle {
    property var name
    property var path

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
        source: path.startsWith("http") ? path : "file:///" + path

        asynchronous: true
    }

    Label {
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

    BusyIndicator {
        visible: preview_image.status !== Image.Ready
        anchors.centerIn: parent
    }

    MouseArea {
        anchors.fill: parent
        id: mouse_area

        onClicked: {
            image.source = path.startsWith("http") ? path : "file:///" + path
            image.rotation = 0
        }
    }
}
