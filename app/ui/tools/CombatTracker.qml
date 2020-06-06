import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../defines.js" as Defines
import "./combat_tracker"
import "./dice"

Page {
    id: root

    readonly property bool inPortrait: width < height || width < 1200
    property bool dice_enabled: false

    // Top Bar
    header: Rectangle {
        id: top_bar
        height: Defines.TOOLBAR_HEIGHT
        color: palette.alternateBase

        Row {
            id: top_row
            anchors.fill: parent

            padding: 10
            spacing: 10

            CustomToolBarButton {
                iconText: FontAwesome.chevronRight
                buttonText: qsTr("Next")
                onClicked: combat_tracker.next()
                usesFixedWidth: false
            }

            CustomToolBarButton {
                iconText: FontAwesome.plus
                buttonText: qsTr("Add")
                usesFixedWidth: false
                onClicked: {
                    add_rect.visible = !add_rect.visible
                }
            }

            CustomToolBarButton {
                iconText: FontAwesome.dice
                buttonText: qsTr("Dice")
                onClicked: dice_enabled ? dice_enabled = false : dice_enabled = true
                usesFixedWidth: false
            }
        }
    }

    // Left Item
    Item {
        id: left_item
        anchors.left: parent.left
        anchors.right: right_item.visible ? right_item.left : parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        // List Header
        CharacterListHeader {
            id: list_header
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: top_bar.height
        }

        AddCharacterDialog {
            id: add_rect
            visible: false
            anchors.top: list_header.bottom
            anchors.left: parent.left
            anchors.right: parent.right
        }

        // Combatant List
        CharacterListView {
            id: list_view

            anchors.left: parent.left
            anchors.top: add_rect.visible ? add_rect.bottom : list_header.bottom
            anchors.bottom: parent.bottom
            anchors.right: parent.right
        }
    }

    // Right Item
    Item {
        id: right_item
        visible: !inPortrait && dice_enabled

        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.width / 3

        Loader {
            id: combat_dice
            source: "dice/DiceCombatTracker.qml"
            anchors.fill: right_item
            asynchronous: true
        }
    }

    footer: Rectangle {
        id: bottom_bar
        height: bottom_column.height

        Column {
            id: bottom_column
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            Loader {
                id: combat_dice_alt
                source: "DiceAlt.qml"

                anchors.left: parent.left
                anchors.right: parent.right

                visible: inPortrait && dice_enabled
                asynchronous: true
            }

            Rectangle {
                id: bottom_rect
                height: Defines.TOOLBAR_HEIGHT
                anchors.left: parent.left
                anchors.right: parent.right
                color: palette.alternateBase

                Row {
                    id: bottom_row
                    height: parent.height
                    padding: 5
                    leftPadding: 0
                    spacing: 5

                    Row {
                        spacing: 10
                        padding: 10
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        width: 100

                        Label {
                            text: qsTr("Round:")
                            anchors.verticalCenter: parent.verticalCenter
                            font.bold: true
                            font.pointSize: 12
                        }

                        Label {
                            text: combat_tracker.currentRound
                            anchors.verticalCenter: parent.verticalCenter
                            font.bold: true
                            font.pointSize: 12
                        }
                    }

                    CustomToolBarButton {
                        id: reset_button
                        iconText: FontAwesome.undo
                        buttonText: qsTr("Reset")
                        usesFixedWidth: false
                        onClicked: combat_tracker.resetRounds()
                    }

                    CustomToolBarButton {
                        id: clear_button
                        iconText: FontAwesome.times
                        buttonText: qsTr("Clear")
                        usesFixedWidth: false
                        onClicked: combat_tracker.clear()
                    }
                }
            }
        }
    }
}
