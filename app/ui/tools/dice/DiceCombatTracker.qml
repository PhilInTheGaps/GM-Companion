pragma ComponentBehavior: Bound

import QtQuick
import ".."
import "../.."

Item {
    id: root

    Rectangle {
        id: sidebar
        color: palette.alternateBase

        width: Sizes.toolbarWidth
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        Column {
            id: dice_col
            anchors.fill: parent
            anchors.margins: 5
            spacing: 10

            Repeater {
                model: dice.diceSides

                DiceButton {
                    required property int modelData

                    sides: modelData

                    anchors.left: dice_col.left
                    anchors.right: dice_col.right

                    isCurrentType: dice.diceTypeSpinBox.value === sides
                    onClicked: dice.diceTypeSpinBox.value = sides
                }
            }
        }
    }

    Dice {
        id: dice
        combat_tracker_mode: true
        anchors.left: parent.left
        anchors.right: sidebar.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
    }
}
