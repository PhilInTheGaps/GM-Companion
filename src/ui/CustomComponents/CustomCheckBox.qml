import QtQuick 2.9
import QtQuick.Controls 2.2
import FontAwesome 2.0

CheckBox {
    id: root
    checked: true
    hoverEnabled: true

    property var color: palette.button

    indicator: Label {
        font.pixelSize: 26
        x: root.leftPadding
        y: parent.height / 2 - height / 2
        color: root.color

        text: FontAwesome.square
        font.family: FontAwesome.familyRegular
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter

        Label {
            text: FontAwesome.check
            anchors.centerIn: parent
            color: root.down ? palette.mid : (root.hovered ? palette.light : palette.buttonText)
            visible: root.checked
            font.pixelSize: 14
            font.family: FontAwesome.familySolid

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
    }

    contentItem: Text {
        text: root.text
        font: root.font
        opacity: enabled ? 1.0 : 0.3
        color: root.down ? palette.midlight : palette.text
        verticalAlignment: Text.AlignVCenter
        leftPadding: root.indicator.width + root.spacing
    }
}
