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

        onCombatantHealthChanged: combatant_health.text = combatantHealth

        onCombatantStatusChanged: combatant_status.text = combatantStatus

        onCurrentRoundChanged: current_round_text.text = qsTr("Round: ")
    }

    Row {
        anchors.fill: parent
        padding: 5

        Column {
            width: parent.width / 2
            height: parent.height
            spacing: 5

            Flow {
                id: combat_tracker_controls
                width: parent.width
                spacing: 5

                Frame {
                    id: current_round_frame
                    padding: 5

                    Text {
                        id: current_round_text
                        text: "Round: 1"
                    }
                }

                Button {
                    id: next_combatant_button
                    text: "Next Combatant"
                    height: current_round_frame.height

                    onClicked: combat_tracker.nextCombatant()
                }

                Button {
                    text: "Reset Rounds"
                    height: current_round_frame.height
                }

                Button {
                    text: qsTr("Clear Table")
                    height: current_round_frame.height
                }
            }

            Frame {
                width: parent.width
                height: 100

                Grid {
                    columns: 4
                    rows: 2
                    spacing: 5

                    anchors.fill: parent

                    Text {
                        text: "Name"
                    }

                    Text {
                        text: "INI"
                    }

                    Text {
                        text: "Health"
                    }

                    Text {
                        text: qsTr("Status")
                    }

                    Text {
                        id: combatant_name
                        text: "No Combatant"
                    }

                    SpinBox {
                        id: combatant_ini
                    }

                    TextField {
                        id: combatant_health
                        width: 100

                    }

                    TextField {
                        id: combatant_status
                        width: 100
                    }
                }
            }

            TableView {
                id: combat_tracker_list
                width: parent.width
//                height: parent.height - combat_tracker_controls.height - combatant_details_row.height
//                selectionMode: SelectionMode.NoSelection

                clip: true
                alternatingRowColors: false

                TableViewColumn {
                    role: "name"
                    title: "Name"
                    width: 200
                }

                TableViewColumn {
                    role: "ini"
                    title: "INI"
                    width: 50
                }

                TableViewColumn {
                    role: "health"
                    title: "Health"
                    width: 100
                }

                TableViewColumn {
                    role: "status"
                    title: "Status"
                    width: 100
                }

                model: ListModel {
                    id: combat_tracker_list_model

//                    ListElement {
//                        name: "Frank Lightspeed"
//                        ini: 24
//                        health: "12 / 14"
//                        status: "Alive"
//                    }

//                    ListElement {
//                        name: "Berythag Appelfass"
//                        ini: 35
//                        health: "20 / 20"
//                        status: "Knocked Out"
//                    }
                }
            }

            Row {
                id: combatant_details_row
                spacing: 5

                Button {
                    id: combatant_add_button
                    text: qsTr("Add")

                    onClicked: combat_tracker.addCombatant(add_name_field.text, add_ini_spinbox.value, add_health_spinbox.value)
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
//                    placeholderText: qsTr("INI")
                }

                SpinBox {
                    id: add_health_spinbox
                    width: 100
                    height: combatant_add_button.height
//                    placeholderText: qsTr("Health")
                }
            }
        }

        Dice {
            id: combat_dice
            height: parent.height
        }
    }
}
