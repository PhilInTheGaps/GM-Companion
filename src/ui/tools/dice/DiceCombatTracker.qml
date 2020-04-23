import QtQuick 2.9
import QtQuick.Controls 2.2

import ".."
import "../combat_tracker"
import "../../defines.js" as Defines

Item {
    id: root

    Rectangle {
        id: sidebar
        color: palette.alternateBase

        width: Defines.TOOLBAR_WIDTH
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
                    sides: modelData
                    width: dice_col.width
                    height: width
                    isCurrentType: dice.diceTypeSpinBox.value == sides
                    onClicked: dice.diceTypeSpinBox.value = sides
                }
            }
        }
    }

    Dice {
        id: dice
        height: minimalHeight
        combat_tracker_mode: true
        anchors.left: parent.left
        anchors.right: sidebar.left
        anchors.top: parent.top
    }

    EffectPage {
        id: effect_page
        anchors.top: dice.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: sidebar.left
        visible: !inPortrait && dice_enabled && hasTables
    }
}
