import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2

Column {
    property var name
    property var path

    anchors.left: parent.left
    anchors.right: parent.right

    BusyIndicator {
        visible: image.status !== Image.Ready
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Item {
        anchors.left: parent.left
        anchors.right: parent.right
        height: image.height

        Image {
            id: image
            anchors.left: parent.left
            anchors.right: parent.right

            sourceSize.width: width
            sourceSize.height: height
            source: path.startsWith("http") ? path : "file:///" + path

            asynchronous: true
        }

        MouseArea {
            anchors.fill: parent
            id: mouse_area

            onClicked: {
                maps_image.source = path.startsWith(
                            "http") ? path : "file:///" + path

                maps_image.rotation = 0
            }
        }
    }
    Text {
        text: parent.name
        color: color_scheme.textColor

        width: parent.width - 10
        wrapMode: Text.WordWrap
        elide: Text.ElideRight
    }
}
