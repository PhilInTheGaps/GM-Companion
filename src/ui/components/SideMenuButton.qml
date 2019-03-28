import QtQuick 2.9
import QtQuick.Controls 2.2
import FontAwesome 2.0

Button {
    property string tool_name
    property string icon_source
    property string fa_icon
    property bool current_tool: false

    id: button
    anchors.left: parent.left
    anchors.right: parent.right
    height: platform.isAndroid ? drawer.height / 15 : 45
    hoverEnabled: true

    background: Rectangle {
        implicitHeight: 40
        visible: settings_tool.classicIcons && parent.down
        color: "white"
    }

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
            source: icon_source
            visible: settings_tool.classicIcons
        }

        // New Icons
        Label {
            visible: !settings_tool.classicIcons
            width: parent.iconWidth
            height: width
            text: fa_icon
            font.family: FontAwesome.familySolid
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: button.down ? "grey" : button.hovered ? "darkgrey" : color_scheme.toolbarTextColor
            font.pixelSize: height - 10
        }

        Text {
            text: button.tool_name
            font.pixelSize: platform.isAndroid ? parent.height / 2 : 17
            visible: !inPortrait && settings_tool.showToolNames

            width: parent.width - parent.spacing - parent.padding * 2 - parent.iconWidth
            clip: true
            elide: Text.ElideRight

            opacity: enabled ? 1.0 : 0.3
            color: button.down ? "grey" : "white"

            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Rectangle {
        visible: current_tool
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 5
        anchors.bottomMargin: 0

        color: "white"
        height: 1
    }
}
