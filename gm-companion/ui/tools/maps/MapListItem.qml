import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import gm.companion.colorscheme 1.0

Column {
    property var name
    property var path
    signal clicked(string path)

    width: parent.width - parent.padding * 2

    ColorScheme {
        id: color_scheme
    }

    BusyIndicator {
        visible: image.status !== Image.Ready
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Item {
        width: parent.width
        height: image.height

        Image {
            id: image
            width: parent.width
            sourceSize.width: width
            sourceSize.height: height
            source: "file:///" + path
            asynchronous: true
        }

        MouseArea {
            anchors.fill: parent
            id: mouse_area

            onClicked: parent.parent.clicked(parent.parent.path)
        }
    }
    Text {
        text: name
        color: color_scheme.textColor

        width: parent.width - 10
        wrapMode: Text.WordWrap
        elide: Text.ElideRight
    }
}
