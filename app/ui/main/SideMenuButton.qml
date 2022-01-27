import QtQuick 2.9
import QtQuick.Controls 2.2
import FontAwesome 2.0
import QtGraphicalEffects 1.0
import "../defines.js" as Defines

Button {
    id: button

    property string toolName
    property string iconSource
    property string faIcon
    property string altColor
    property bool currentTool: false
    property bool useAltColor: false

    anchors.left: parent.left
    anchors.right: parent.right
    height: Defines.TOOLBAR_HEIGHT
    hoverEnabled: true

    background: Item {}

    font.bold: false

    Row {
        anchors.fill: parent
        padding: 5
        spacing: 5

        property int iconWidth: height - padding * 2

        // Classic Icons
        Image {
            id: button_icon
            fillMode: Image.PreserveAspectFit
            anchors.verticalCenter: parent.verticalCenter
            sourceSize.width: parent.iconWidth
            sourceSize.height: parent.iconWidth
            height: sourceSize.height
            width: height
            source: iconSource
            visible: settings_manager.classicIcons
            asynchronous: true

            ColorOverlay {
                anchors.fill: parent
                source: parent
                visible: button.pressed || button.hovered
                color: {
                    if (button.pressed) {
                        Qt.rgba(0, 0, 0, 0.4)
                    } else if (button.hovered) {
                        Qt.rgba(0, 0, 0, 0.2)
                    } else {
                        "white"
                    }
                }
            }
        }

        // New Icons
        Label {
            visible: !button_icon.visible
            width: parent.iconWidth
            height: width
            text: faIcon
            font.family: FontAwesome.familySolid
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: button.down ? "grey" : button.hovered ? "darkgrey" : button.useAltColor ? button.altColor : palette.text
            font.pixelSize: height - 10
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            text: button.toolName
            font.pixelSize: 16
            visible: settings_manager.showToolNames

            width: parent.width - parent.spacing - parent.padding * 2 - parent.iconWidth
            clip: true
            elide: Text.ElideRight

            opacity: enabled ? 1.0 : 0.3
            color: button.down ? "grey" : button.hovered ? "darkgrey" : palette.text

            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
