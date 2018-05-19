import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls 1.4 as Controls1_4

import gm.companion.combattracker 1.0
import gm.companion.colorscheme 1.0
import gm.companion.platforms 1.0

Page {
    id: combat_tracker

    readonly property bool inPortrait: width < height

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

    PlatformDetails {
        id: platform
    }

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    Dialog {
        id: add_combatant_dialog
        title: qsTr("Add Combatant")
        width: parent.width / 1.5
        height: parent.height / 1.5
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        contentItem: Column {
            id: dialog_column
            spacing: 5

            Grid {
                columns: 3
                rows: 3
                spacing: 5
                anchors.horizontalCenter: parent.horizontalCenter

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
                    width: dialog_column.width / 4
                    height: current_round_frame.height
                    placeholderText: qsTr("Name")
                    selectByMouse: true
                }

                SpinBox {
                    id: add_ini_spinbox
                    width: dialog_column.width / 4
                    height: current_round_frame.height
                    editable: true
                }

                SpinBox {
                    id: add_health_spinbox
                    width: dialog_column.width / 4
                    height: current_round_frame.height
                    editable: true
                }
            }
        }

        standardButtons: Dialog.Ok | Dialog.Cancel

        onAccepted: {
            combat_tracker_tool.addCombatant(add_name_field.text,
                                             add_ini_spinbox.value,
                                             add_health_spinbox.value)
        }
    }

    Row {
        id: row
        anchors.fill: parent
        spacing: 5
        padding: 5

        Column {
            id: left_column
            height: parent.height - parent.padding * 2
            width: platform.isAndroid ? parent.width / 5 : 150
            spacing: 5

            Frame {
                id: current_round_frame
                padding: 5
                width: parent.width
                height: platform.isAndroid ? width / 6 : 40

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
                width: parent.width
                height: platform.isAndroid ? width / 6 : 40

                onClicked: combat_tracker_tool.nextCombatant()
            }

            Button {
                id: add_combatant_button
                text: qsTr("Add Combatant")
                width: parent.width
                height: platform.isAndroid ? width / 6 : 40

                onClicked: add_combatant_dialog.open()
            }

            Button {
                text: qsTr("Reset Rounds")
                width: parent.width
                height: platform.isAndroid ? width / 6 : 40

                onClicked: combat_tracker_tool.resetRounds()
            }

            Button {
                text: qsTr("Clear Table")
                width: parent.width
                height: platform.isAndroid ? width / 6 : 40

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
                width: parent.width
                height: platform.isAndroid ? width / 6 : 40

                Row {
                    spacing: 5
                    padding: 5
                    anchors.fill: parent

                    Image {
                        id: dice_image
                        source: "../icons/menu/dice.png"

                        width: height
                        height: parent.height - parent.padding * 2
                    }

                    Text {
                        text: qsTr("Toggle Dice")
                        clip: true
                        width: parent.width - dice_image.width - parent.spacing - parent.padding * 2
                        elide: Text.ElideRight
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                onClicked: {
                    combat_dice.visible = !combat_dice.visible
                }
            }
        }
        ScrollView {
            id: combat_tracker_scroll_view
            height: parent.height - parent.padding * 2
            width: parent.width - parent.padding * 2 - parent.spacing - left_column.width
            padding: 0
            clip: true

            Flow {
                width: row.width - row.padding * 2 - row.spacing - left_column.width
                spacing: 5

                Column {
                    id: main_column
                    width: inPortrait || combat_dice.visible
                           == false ? parent.width - parent.padding
                                      * 2 : (parent.width - parent.padding * 2 - parent.spacing) / 2

                    spacing: 5

                    // Current Combatant Information
                    Frame {
                        width: parent.width

                        background: Rectangle {
                            color: color_scheme.primaryButtonColor
                        }

                        ScrollView {
                            anchors.fill: parent
                            clip: true

                            Column {
                                spacing: 5

                                Text {
                                    text: qsTr("Current Combatant")
                                    color: "white"
                                }

                                Frame {
                                    padding: 5
                                    height: next_combatant_button.height
                                    width: parent.width

                                    Text {
                                        id: combatant_name
                                        text: qsTr("No Combatant")
                                        anchors.verticalCenter: parent.verticalCenter
                                        color: "white"
                                    }
                                }

                                Grid {
                                    columns: 4
                                    rows: 2
                                    spacing: 5

                                    Text {
                                        text: qsTr("INI")
                                        color: "white"
                                    }

                                    Text {
                                        text: qsTr("Health")
                                        color: "white"
                                    }

                                    Text {
                                        text: qsTr("Max H.")
                                        color: "white"
                                    }

                                    Text {
                                        text: qsTr("Status")
                                        color: "white"
                                    }

                                    SpinBox {
                                        id: combatant_ini
                                        editable: true
                                        height: next_combatant_button.height

                                        onValueChanged: {
                                            if (combat_tracker_tool.currentIndex > -1) {
                                                combat_tracker_tool.setCombatantIni(
                                                            value)
                                                combat_tracker_list_model.setProperty(
                                                            combat_tracker_tool.currentIndex,
                                                            "ini",
                                                            value.toString())
                                            }
                                        }
                                    }

                                    SpinBox {
                                        id: combatant_health
                                        editable: true
                                        height: next_combatant_button.height
                                        onValueChanged: {
                                            if (combat_tracker_tool.currentIndex > -1) {
                                                combat_tracker_tool.setCombatantHealth(
                                                            value)
                                                combat_tracker_list_model.setProperty(
                                                            combat_tracker_tool.currentIndex,
                                                            "health",
                                                            value + " / " + combat_tracker_tool.combatantMaxHealth)
                                            }
                                        }
                                    }

                                    SpinBox {
                                        id: combatant_max_health
                                        editable: true
                                        height: next_combatant_button.height
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
                                        height: next_combatant_button.height
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

                    Text {
                        text: qsTr("Combatants")
                        color: color_scheme.textColor
                    }

                    // Combatants Table
                    Controls1_4.TableView {
                        id: combat_tracker_list
                        width: parent.width
                        height: parent.height / 2

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
                        height: next_combatant_button.height
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
                    width: inPortrait ? parent.width - parent.padding
                                        * 2 : (parent.width - parent.padding * 2
                                               - parent.spacing) / 2
                    height: inPortrait ? main_column.height : parent.height - parent.padding * 2
                    visible: false
                }
            }
        }
    }
}
