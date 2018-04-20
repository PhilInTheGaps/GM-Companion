import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4 as Controls1_4

import gm.companion.combattracker 1.0
import gm.companion.colorscheme 1.0

Page {
    id: combat_tracker

    CombatTrackerTool {
        id: combat_tracker_tool

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

    ColorScheme {
        id: color_scheme
    }

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    ScrollView {
        id: combat_tracker_scroll_view
        anchors.fill: parent
        clip: true

        Flow {
            width: combat_tracker.width

            padding: 5
            spacing: 5

            Column {
                width: parent.width > combat_dice.width * 2
                       && combat_dice.visible ? parent.width - combat_dice.width
                                                - parent.spacing - parent.padding
                                                * 2 : parent.width - parent.spacing * 2
                spacing: 5

                // Top Control Buttons
                Flow {
                    id: combat_tracker_controls
                    width: parent.width - combat_dice_button.width
                    spacing: 5

                    Frame {
                        id: current_round_frame
                        padding: 5
                        height: combat_tracker.height / 20

                        Text {
                            id: current_round_text
                            text: qsTr("Round: 1")
                            anchors.verticalCenter: parent.verticalCenter
                            color: color_scheme.textColor
                        }
                    }

                    Button {
                        id: next_combatant_button
                        text: qsTr("Next Combatant")
                        height: current_round_frame.height

                        onClicked: combat_tracker_tool.nextCombatant()
                    }

                    Button {
                        text: qsTr("Reset Rounds")
                        height: current_round_frame.height

                        onClicked: combat_tracker_tool.resetRounds()
                    }

                    Button {
                        text: qsTr("Clear Table")
                        height: current_round_frame.height

                        onClicked: {
                            combat_tracker_tool.clearCombatants()
                            combat_tracker_list_model.clear()

                            combatant_name.text = qsTr("No Combatant")
                            combatant_ini.value = 0
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
                        }

                        onClicked: {
                            combat_dice.visible ? combat_dice.visible
                                                  = false : combat_dice.visible = true
                        }
                    }
                }

                Text {
                    text: qsTr("Current Combatant")
                    color: color_scheme.textColor
                }

                // Current Combatant Information
                Frame {
                    width: parent.width

                    ScrollView {
                        anchors.fill: parent
                        clip: true

                        Column {
                            spacing: 5

                            Text {
                                text: qsTr("Name")
                                color: color_scheme.textColor
                            }

                            Frame {
                                padding: 5
                                height: combatant_ini.height

                                Text {
                                    id: combatant_name
                                    text: qsTr("No Combatant")
                                    anchors.verticalCenter: parent.verticalCenter
                                    color: color_scheme.textColor
                                }
                            }

                            Grid {
                                columns: 4
                                rows: 2
                                spacing: 5

                                Text {
                                    text: qsTr("INI")
                                    color: color_scheme.textColor
                                }

                                Text {
                                    text: qsTr("Health")
                                    color: color_scheme.textColor
                                }

                                Text {
                                    text: qsTr("Max H.")
                                    color: color_scheme.textColor
                                }

                                Text {
                                    text: qsTr("Status")
                                    color: color_scheme.textColor
                                }

                                SpinBox {
                                    id: combatant_ini
                                    editable: true

                                    onValueChanged: {
                                        if (combat_tracker_tool.currentIndex > -1) {
                                            combat_tracker_tool.setCombatantIni(
                                                        value)
                                            combat_tracker_list_model.setProperty(
                                                        combat_tracker_tool.currentIndex,
                                                        "ini", value.toString())
                                        }
                                    }
                                }

                                SpinBox {
                                    id: combatant_health
                                    editable: true
                                    onValueChanged: {
                                        if (combat_tracker_tool.currentIndex > -1) {
                                            combat_tracker_tool.setCombatantHealth(
                                                        value)
                                            combat_tracker_list_model.setProperty(
                                                        combat_tracker_tool.currentIndex,
                                                        "health", value + " / "
                                                        + combat_tracker_tool.combatantMaxHealth)
                                        }
                                    }
                                }

                                SpinBox {
                                    id: combatant_max_health
                                    editable: true
                                    onValueChanged: {
                                        if (combat_tracker_tool.currentIndex > -1) {
                                            combat_tracker_tool.setCombatantMaxHealth(
                                                        value)
                                            combat_tracker_list_model.setProperty(
                                                        combat_tracker_tool.currentIndex,
                                                        "health",
                                                        combat_tracker_tool.combatantHealth
                                                        + " / " + value)
                                        }
                                    }
                                }

                                TextField {
                                    id: combatant_status
                                    width: 100
                                    selectByMouse: true

                                    onTextChanged: {
                                        if (combat_tracker_tool.currentIndex > -1) {
                                            combat_tracker_tool.setCombatantStatus(
                                                        text)

                                            combat_tracker_list_model.setProperty(
                                                        combat_tracker_tool.currentIndex,
                                                        "status", text)
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                // Add Combatant stuff
                Frame {
                    width: parent.width

                    ScrollView {
                        anchors.fill: parent
                        clip: true

                        Grid {
                            columns: 3
                            rows: 3

                            spacing: 5
                            Text {
                                text: qsTr("Name")
                                color: color_scheme.textColor
                            }

                            Text {
                                text: qsTr("INI")
                                color: color_scheme.textColor
                            }

                            Text {
                                text: qsTr("Health")
                                color: color_scheme.textColor
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
                                editable: true
                            }

                            SpinBox {
                                id: add_health_spinbox
                                width: 100
                                height: combatant_add_button.height
                                editable: true
                            }

                            Button {
                                id: combatant_add_button
                                text: qsTr("Add")

                                onClicked: {
                                    combat_tracker_tool.addCombatant(
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
                    color: color_scheme.textColor
                }

                // Combatants Table
                Controls1_4.TableView {
                    id: combat_tracker_list
                    width: parent.width
                    height: parent.height / 3

                    clip: true
                    alternatingRowColors: false

                    Controls1_4.TableViewColumn {
                        role: "name"
                        title: qsTr("Name")
                        width: 200
                    }

                    Controls1_4.TableViewColumn {
                        role: "ini"
                        title: qsTr("INI")
                        width: 50
                    }

                    Controls1_4.TableViewColumn {
                        role: "health"
                        title: qsTr("Health")
                        width: 100
                    }

                    Controls1_4.TableViewColumn {
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

                            combat_tracker_tool.removeCombatant(index)
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
