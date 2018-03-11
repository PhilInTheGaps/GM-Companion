import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4

import gm.companion.combattracker 1.0

Page {
    id: combat_tracker_tool

    CombatTrackerTool {
        id: combat_tracker

        onCombatantNameChanged: combatant_name.text = combatantName

        onCombatantIniChanged: combatant_ini.value = combatantIni

        onCombatantHealthChanged: {
            combatant_health.value = combatantHealth
            combatant_max_health.value = combatantMaxHealth
        }

        onCombatantStatusChanged: combatant_status.text = combatantStatus

        onCurrentRoundChanged: current_round_text.text = qsTr(
                                   "Round: ") + currentRound

        onLatestAddedCombatantChanged: {
            combat_tracker_list_model.append({
                                                 name: latestAddedCombatant[0],
                                                 ini: latestAddedCombatant[1],
                                                 health: latestAddedCombatant[2],
                                                 status: qsTr("Alive")
                                             })
        }
    }

    ScrollView {
        id: combat_tracker_scroll_view
        anchors.fill: parent
        flickableItem.interactive: true
        clip: true
        flickableItem.flickableDirection: Flickable.VerticalFlick

        // Top Control Buttons
        Flow {
            width: combat_tracker_tool.width

            padding: 5
            spacing: 5

            Column {
                width: combat_tracker_tool.width
                       > combat_tracker_tool.height ? parent.width / 2 : parent.width
                spacing: 5

                Flow {
                    id: combat_tracker_controls
                    width: parent.width - combat_dice_button.width
                    spacing: 5

                    Frame {
                        id: current_round_frame
                        padding: 5

                        Text {
                            id: current_round_text
                            text: qsTr("Round: 1")
                        }
                    }

                    Button {
                        id: next_combatant_button
                        text: qsTr("Next Combatant")
                        height: current_round_frame.height

                        onClicked: combat_tracker.nextCombatant()
                    }

                    Button {
                        text: qsTr("Reset Rounds")
                        height: current_round_frame.height

                        onClicked: combat_tracker.resetRounds()
                    }

                    Button {
                        text: qsTr("Clear Table")
                        height: current_round_frame.height

                        onClicked: {
                            combat_tracker.clearCombatants()
                            combat_tracker_list_model.clear()

                            combatant_name.text = qsTr("No Combatant")
                            combatant_ini.text = 0
                            combatant_health.value = 0
                            combatant_max_health.value = 0
                            combatant_status.text = ""
                        }
                    }

                    Button {
                        id: combat_dice_button
                        width: current_round_frame.height
                        height: width

                        x: parent.width - width

                        Image {
                            source: "../icons/menu/dice.png"

                            width: parent.width
                            height: parent.height

                            sourceSize.height: height
                            sourceSize.width: width
                        }

                        onClicked: {
                            combat_dice.visible ? combat_dice.visible
                                                  = false : combat_dice.visible = true
                        }
                    }
                }

                Text {
                    text: qsTr("Current Combatant")
                }

                // Current Combatant Information
                Frame {
                    width: parent.width
                    height: 100

                    ScrollView {
                        anchors.fill: parent
                        flickableItem.interactive: true
                        flickableItem.clip: true

                        Grid {
                            columns: 5
                            rows: 2
                            spacing: 5

                            Text {
                                text: qsTr("Name")
                            }

                            Text {
                                text: qsTr("INI")
                            }

                            Text {
                                text: qsTr("Health")
                            }

                            Text {
                                text: qsTr("Max H.")
                            }

                            Text {
                                text: qsTr("Status")
                            }

                            Frame {
                                padding: 5

                                Text {
                                    id: combatant_name
                                    text: qsTr("No Combatant")
                                }
                            }

                            SpinBox {
                                id: combatant_ini
                                width: 50

                                onValueChanged: {
                                    if (combat_tracker.currentIndex > -1) {
                                        combat_tracker.setCombatantIni(value)
                                        combat_tracker_list_model.setProperty(
                                                    combat_tracker.currentIndex,
                                                    "ini", value.toString())
                                    }
                                }
                            }

                            SpinBox {
                                id: combatant_health
                                width: 50

                                onValueChanged: {
                                    if (combat_tracker.currentIndex > -1) {
                                        combat_tracker.setCombatantHealth(value)
                                        combat_tracker_list_model.setProperty(
                                                    combat_tracker.currentIndex,
                                                    "health", value + " / "
                                                    + combat_tracker.combatantMaxHealth)
                                    }
                                }
                            }

                            SpinBox {
                                id: combatant_max_health
                                width: 50

                                onValueChanged: {
                                    if (combat_tracker.currentIndex > -1) {
                                        combat_tracker.setCombatantMaxHealth(
                                                    value)
                                        combat_tracker_list_model.setProperty(
                                                    combat_tracker.currentIndex,
                                                    "health",
                                                    combat_tracker.combatantHealth + " / " + value)
                                    }
                                }
                            }

                            TextField {
                                id: combatant_status
                                width: 100

                                onTextChanged: {
                                    if (combat_tracker.currentIndex > -1) {
                                        combat_tracker.setCombatantStatus(text)

                                        combat_tracker_list_model.setProperty(
                                                    combat_tracker.currentIndex,
                                                    "status", text)
                                    }
                                }
                            }
                        }
                    }
                }

                // Add Combatant stuff
                Frame {
                    width: parent.width
                    height: 125

                    ScrollView {
                        anchors.fill: parent
                        flickableItem.interactive: true
                        flickableItem.clip: true

                        Grid {
                            columns: 3
                            rows: 3

                            spacing: 5
                            Text {
                                text: qsTr("Name")
                            }

                            Text {
                                text: qsTr("INI")
                            }

                            Text {
                                text: qsTr("Health")
                            }

                            TextField {
                                id: add_name_field
                                width: 100
                                height: combatant_add_button.height
                                placeholderText: qsTr("Name")
                            }

                            SpinBox {
                                id: add_ini_spinbox
                                width: 100
                                height: combatant_add_button.height
                            }

                            SpinBox {
                                id: add_health_spinbox
                                width: 100
                                height: combatant_add_button.height
                            }

                            Button {
                                id: combatant_add_button
                                text: qsTr("Add")

                                onClicked: {
                                    combat_tracker.addCombatant(
                                                add_name_field.text,
                                                add_ini_spinbox.value,
                                                add_health_spinbox.value)
                                }
                            }
                        }
                    }
                }

                Text {
                    text: qsTr("Combatants")
                }

                // Combatants Table
                TableView {
                    id: combat_tracker_list
                    width: parent.width
                    height: parent.height / 3

                    clip: true
                    alternatingRowColors: false

                    TableViewColumn {
                        role: "name"
                        title: qsTr("Name")
                        width: 200
                    }

                    TableViewColumn {
                        role: "ini"
                        title: qsTr("INI")
                        width: 50
                    }

                    TableViewColumn {
                        role: "health"
                        title: qsTr("Health")
                        width: 100
                    }

                    TableViewColumn {
                        role: "status"
                        title: qsTr("Status")
                        width: 170
                    }

                    model: ListModel {
                        id: combat_tracker_list_model
                    }
                }

                Button {
                    text: qsTr("Delete Selected Combatant")
                    onClicked: {

                        var index = combat_tracker_list.currentRow

                        if (index > -1
                                && index <= combat_tracker_list.rowCount - 1) {
                            combat_tracker_list_model.remove(index)

                            combat_tracker.removeCombatant(index)
                        }
                    }
                }
            }

            Dice {
                id: combat_dice
                visible: false
            }
        }
    }
}
