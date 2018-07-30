import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import gm.companion.colorscheme 1.0

Rectangle {
    id: character_button
    property var character_name
    property bool active: true

    signal clicked(string character_name)
    signal toggle_active_clicked(string character_name, bool active)

    width: parent ? parent.width : 0
    height: platform.isAndroid ? width / 6 : 40

    ColorScheme {
        id: color_scheme
    }

    Row {
        anchors.fill: parent
        padding: 5

        Text {
            text: character_name
            color: mouse_area.pressed ? "black" : color_scheme.buttonTextColor

            width: toggle_active_button.x - parent.padding - 5
            anchors.verticalCenter: parent.verticalCenter
            clip: true
            elide: Text.ElideRight
        }
    }

    color: mouse_area.pressed ? "white" : color_scheme.primaryButtonColor

    MouseArea {
        anchors.fill: parent
        id: mouse_area

        onClicked: parent.clicked(parent.character_name)
    }

    Button {
        id: toggle_active_button
        width: parent.height - 10
        height: width

        x: parent.width - width - 5
        anchors.verticalCenter: parent.verticalCenter

        ToolTip.text: parent.active ? "Make Inactive" : "Make Active"
        ToolTip.visible: hovered
        hoverEnabled: true

        background: Rectangle {
            color: "transparent"
        }

        Image {
            source: character_button.active ? "/icons/menu/x_sign_bright.png" : "/icons/media/playBackwards_bright.png"
            width: parent.height * 0.9
            height: width

            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter

            sourceSize.width: width
            sourceSize.height: height
        }

        onClicked: parent.toggle_active_clicked(parent.character_name,
                                                parent.active)
    }
}
