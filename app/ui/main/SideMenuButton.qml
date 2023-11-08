import QtQuick
import QtQuick.Controls
import IconFonts
import common
import ".."

Button {
    id: root

    property string toolName
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

    ToolTip.delay: 1000
    ToolTip.visible: root.hovered && !SettingsManager.showToolNames
    ToolTip.text: root.toolName

    Row {
        anchors.fill: parent
        padding: 5
        spacing: 5

        readonly property int iconWidth: height - padding * 2

        Label {
            width: parent.iconWidth
            height: width
            text: root.faIcon
            font.family: FontAwesome.fontSolid.family
            font.styleName: FontAwesome.fontSolid.styleName
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: root.down ? "grey" : root.hovered ? "darkgrey" : root.useAltColor ? root.altColor : palette.text
            font.pixelSize: height - 10
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            text: root.toolName
            font.pixelSize: 16
            visible: SettingsManager.showToolNames

            width: parent.width - parent.spacing - parent.padding * 2 - parent.iconWidth
            clip: true
            elide: Text.ElideRight

            opacity: enabled ? 1.0 : 0.3
            color: root.down ? "grey" : root.hovered ? "darkgrey" : palette.text

            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
