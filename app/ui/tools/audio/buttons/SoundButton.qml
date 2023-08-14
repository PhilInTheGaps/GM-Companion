import QtQuick 2.9
import QtQuick.Controls 2.2
import IconFonts
import "../../../defines.js" as Defines

Button {
    id: root
    property var element
    property var element_icon

    height: Defines.TOOLBAR_HEIGHT
    hoverEnabled: true

    Image {
        id: thumbnail
        property bool counter: false

        source: "image://audioElementIcons/" + element_icon.imageId
        anchors.fill: parent

        asynchronous: true
        cache: false

        sourceSize.width: 400
        sourceSize.height: 400

        fillMode: Image.PreserveAspectCrop

        function reload() {
            counter = !counter
            source = "image://audioElementIcons/" + element_icon.imageId + "?r=" + counter
        }

        Connections {
            target: element_icon
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
            text: element
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
            color: parent.parent.pressed ? "darkgrey" : "white"
        }
    }

    background: Rectangle {
        color: "transparent"
    }

    onClicked: {
        audio_tool.stopSound(element)
    }
}
