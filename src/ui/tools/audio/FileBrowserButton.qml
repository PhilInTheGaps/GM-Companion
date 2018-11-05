import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import FontAwesome 2.0
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

        Text {
            id: plus_icon
            text: FontAwesome.plus
            font.pixelSize: parent.height - 10
            font.family: FontAwesome.familySolid

            anchors.verticalCenter: parent.verticalCenter
            visible: type !== 3
            color: "white"
        }

        Text {
            id: icon
            font.pixelSize: (parent.height - parent.padding * 2) * 0.8
            font.family: FontAwesome.familySolid
            anchors.verticalCenter: parent.verticalCenter

            color: "white"

            Component.onCompleted: {
                switch (type) {
                case 0:
                    text = FontAwesome.music
                    break
                case 1:
                    text = FontAwesome.drum
                    break
                case 2:
                    text = FontAwesome.broadcastTower
                    break
                case 3:
                    text = FontAwesome.bars
                    break
                }
            }
        }

        Text {
            text: element
            color: mouse_area.pressed ? "black" : color_scheme.buttonTextColor
            width: parent.width - parent.spacing - parent.padding * 3 - icon.width - plus_icon.width
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
