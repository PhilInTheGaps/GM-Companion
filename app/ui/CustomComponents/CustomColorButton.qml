import QtQuick
import IconFonts

Rectangle {
    id: root

    property bool enableReset: false

    signal clicked
    signal resetClicked

    implicitWidth: 100
    implicitHeight: 40

    color: "blue"

    border.width: mouse_area.containsMouse ? 1 : 0
    border.color: mouse_area.pressed ? CCColors.buttonPressed : palette.button

    MouseArea {
        id: mouse_area
        anchors.fill: parent
        hoverEnabled: true

        onClicked: root.clicked()
    }

    CustomButton {
        visible: root.enableReset
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        iconText: FontAwesome.rotateLeft
        onClicked: root.resetClicked()
    }
}
