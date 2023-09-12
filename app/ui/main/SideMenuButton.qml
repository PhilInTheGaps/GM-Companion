import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import IconFonts
import common
import ".."

Button {
    id: button

    property string toolName
    property string iconSource
    property string faIcon
    property string altColor
    property bool currentTool: false
    property bool useAltColor: false

    anchors.left: parent ? parent.left : undefined
    anchors.right: parent ? parent.right : undefined
    height: Sizes.toolbarHeight
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
            source: button.iconSource
            visible: SettingsManager.classicIcons && button.iconSource !== ""
            asynchronous: true

            MultiEffect {
                source: button_icon
                anchors.fill: button_icon

                visible: button.pressed || button.hovered
                colorization: 1
                colorizationColor: {
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
            text: button.faIcon
            font.family: FontAwesome.fontSolid.family
            font.styleName: FontAwesome.fontSolid.styleName
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: button.down ? "grey" : button.hovered ? "darkgrey" : button.useAltColor ? button.altColor : palette.text
            font.pixelSize: height - 10
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            text: button.toolName
            font.pixelSize: 16
            visible: SettingsManager.showToolNames

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
