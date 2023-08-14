import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import IconFonts

Rectangle {
    id: character_button
    property var characterName
    property bool active: true

    signal clicked(string character_name)
    signal toggleActiveClicked

    anchors.left: parent.left
    anchors.right: parent.right
    height: 40

    Row {
        anchors.fill: parent
        padding: 5

        Text {
            text: characterName
            color: mouse_area.pressed ? "black" : palette.buttonText

            width: toggle_active_button.x - parent.padding - 5
            anchors.verticalCenter: parent.verticalCenter
            clip: true
            elide: Text.ElideRight
        }
    }

    color: mouse_area.pressed ? "white" : palette.button

    MouseArea {
        anchors.fill: parent
        id: mouse_area
        hoverEnabled: true

        onClicked: parent.clicked(parent.characterName)
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

        visible: mouse_area.containsMouse || hovered

        background: Rectangle {
            color: "transparent"
        }

        Text {
            text: character_button.active ? FontAwesome.xmark : FontAwesome.plus
            font.family: FontAwesome.fontSolid.family
            font.styleName: FontAwesome.fontSolid.styleName
            font.pointSize: 15
            anchors.centerIn: parent
            color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
        }

        onClicked: parent.toggle_active_clicked()
    }
}
