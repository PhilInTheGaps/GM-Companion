import QtQuick 2.9
import QtQuick.Controls 2.2

import ".."
import "../combat_tracker"
import gm.companion.colorscheme 1.0

Item {
    id: root

    ColorScheme {
        id: colors
    }

    Rectangle {
        id: spacer
        anchors.top: parent.top
        height: colors.toolbarHeight
    }

    Rectangle {
        id: sidebar
        color: colors.toolbarColor

        width: colors.toolbarHeight
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        Column {
            id: dice_col
            anchors.fill: parent
            anchors.margins: 5
            anchors.topMargin: colors.toolbarHeight + 5
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
        anchors.top: spacer.bottom
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
