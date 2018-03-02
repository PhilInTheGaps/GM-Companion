import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4

Page {
    id: combat_tracker_tool

    Row {
        anchors.fill: parent

        Column {
            width: parent.width / 2
            height: parent.height

            Row {
                id: combat_tracker_controls
                width: parent.width
                spacing: 5
                padding: 5

                Text {
                    text: "Round: 1"
                }

                Button {
                    text: "Next Combatant"
                }

                Button {
                    text: "Reset Rounds"
                }
            }

            TableView {
                id: combat_tracker_list
                width: parent.width
                height: parent.height - combat_tracker_controls.height

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

                    ListElement {
                        name: "Frank Lightspeed"
                        ini: 24
                        health: "12 / 14"
                        status: "Alive"
                    }

                    ListElement {
                        name: "Berythag Appelfass"
                        ini: 35
                        health: "20 / 20"
                        status: "Knocked Out"
                    }
                }
            }
        }

        Dice {
            id: combat_dice
            height: parent.height
        }
    }
}
