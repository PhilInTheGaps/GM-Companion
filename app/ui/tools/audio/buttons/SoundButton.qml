import QtQuick
import QtQuick.Controls
import IconFonts
import src
import "../../.."

Button {
    id: root
    property string elementName
    property AudioThumbnail elementThumbnail

    height: Sizes.toolbarHeight
    hoverEnabled: true

    Image {
        id: thumbnail
        property bool counter: false

        source: "image://audioElementIcons/" + root.elementThumbnail.imageId
        anchors.fill: parent

        asynchronous: true
        cache: false

        sourceSize.width: 400
        sourceSize.height: 400

        fillMode: Image.PreserveAspectCrop

        function reload() {
            counter = !counter
            source = "image://audioElementIcons/" + root.elementThumbnail.imageId + "?r=" + counter
        }

        Connections {
            target: root.elementThumbnail
            function onThumbnailChanged() {
                thumbnail.reload()
            }
        }
    }

    Rectangle {
        id: text_overlay
        color: Qt.rgba(0, 0, 0, 0.4)
        anchors.fill: parent

        Text {
            id: text
            text: root.elementName
            color: "white"

            anchors.fill: parent
            anchors.margins: 5

            //        font.pointSize: 12
            clip: true
            elide: Text.ElideRight

            verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle {
        id: overlay
        anchors.fill: parent
        color: Qt.rgba(0, 0, 0, 0.5)
        visible: parent.hovered

        Text {
            text: FontAwesome.xmark
            font.pixelSize: parent.height - 10
            font.family: FontAwesome.fontSolid.family
            font.styleName: FontAwesome.fontSolid.styleName
            anchors.centerIn: parent
            color: root.pressed ? "darkgrey" : "white"
        }
    }

    background: Rectangle {
        color: "transparent"
    }

    onClicked: {
        AudioTool.stopSound(elementName)
    }
}
