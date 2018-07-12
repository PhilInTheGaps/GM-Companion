import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4

import "../../fontawesome"
import gm.companion.colorscheme 1.0

Rectangle {
    id: file_browser_button

    property var element
    property var path
    property var type

    signal clicked(string element, string path)

    width: parent ? parent.width : 0
    height: 30

    ColorScheme {
        id: color_scheme
    }

    Row {
        anchors.fill: parent
        padding: 5
        spacing: 5

        Icon {
            id: plus_icon
            icon: icons.fa_plus
            pointSize: 14
            anchors.verticalCenter: parent.verticalCenter
            visible: type !== 3
            color: "white"
        }

        Image {
            id: image
            width: (parent.height - parent.padding * 2) * 0.8
            height: width
            sourceSize.width: width
            sourceSize.height: height
            anchors.verticalCenter: parent.verticalCenter

            Component.onCompleted: {
                switch (type) {
                case 0:
                    source = "/icons/media/music_bright.png"
                    break
                case 1:
                    source = "/icons/media/sound_bright.png"
                    break
                case 2:
                    source = "/icons/media/radio.png"
                    break
                case 3:
                    source = "/icons/menu/three_bars_white.png"
                    break
                }
            }
        }

        Text {
            text: element
            color: mouse_area.pressed ? "black" : color_scheme.buttonTextColor
            width: parent.width - parent.spacing - parent.padding * 3
                   - image.width - plus_icon.width
            clip: true
            elide: Text.ElideRight
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    color: mouse_area.pressed ? "white" : color_scheme.secondaryButtonColor

    MouseArea {
        anchors.fill: parent
        id: mouse_area

        ToolTip {
            id: tool_tip
            text: type === 3 ? element : "Add: " + element
        }

        onClicked: {
            parent.clicked(parent.element, parent.path)
        }

        hoverEnabled: true

        onEntered: tool_tip.visible = true
        onExited: tool_tip.visible = false
    }
}
