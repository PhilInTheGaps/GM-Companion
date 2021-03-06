import QtQuick 2.9
import QtQuick.Controls 2.2

Dialog {
    id: add_combatant_dialog

    signal addCombatant(string name, int ini, int health, bool sort)
    property bool addAdditionalCombatants: false

    title: qsTr("Add Combatant")
    width: parent.width * 0.8
    height: parent.height * 0.8
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    modal: true

    contentItem: Page {

        padding: 10

        Column {
            id: main_column
            anchors.fill: parent
            spacing: 10

            Row {
                id: list_header
                spacing: 5
                anchors.left: parent.left
                anchors.right: parent.right

                Rectangle {
                    id: header_spacer
                    width: (parent.width - parent.spacing * 3) / 4
                    height: 5
                    color: "transparent"
                }

                Text {
                    text: qsTr("Name")
                    width: (parent.width - parent.spacing * 3) / 4
                    horizontalAlignment: Text.AlignHCenter
                }

                Text {
                    text: qsTr("INI")
                    width: (parent.width - parent.spacing * 3) / 4
                    horizontalAlignment: Text.AlignHCenter
                }

                Text {
                    text: qsTr("Health")
                    width: (parent.width - parent.spacing * 3) / 4
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            Row {
                id: first_row
                spacing: 5
                anchors.left: parent.left
                anchors.right: parent.right

                Item {
                    width: (parent.width - parent.spacing * 3) / 4
                    height: add_more_button.height

                    Button {
                        id: add_more_button
                        text: qsTr("Add More")
                        width: parent.width / 2
                        anchors.right: parent.right
                        onClicked: {
                            if (addAdditionalCombatants) {
                                scroll_view.visible = false
                                addAdditionalCombatants = false
                                text = qsTr("Add More")
                            } else {
                                scroll_view.visible = true
                                addAdditionalCombatants = true
                                text = qsTr("Hide List")
                            }
                        }
                    }
                }

                TextField {
                    id: name_field
                    width: (parent.width - parent.spacing * 3) / 4
                    placeholderText: qsTr("Name")
                    selectByMouse: true
                }

                SpinBox {
                    id: ini_spinbox
                    width: (parent.width - parent.spacing * 3) / 4
                    editable: true

                    from: 0
                    to: 99999
                }

                SpinBox {
                    id: health_spinbox
                    width: (parent.width - parent.spacing * 3) / 4
                    editable: true

                    from: 0
                    to: 99999
                }
            }

            ScrollView {
                id: scroll_view
                anchors.left: parent.left
                anchors.right: parent.right
                height: parent.height - first_row.height - list_header.height - parent.spacing * 2

                visible: false
                clip: true
                contentHeight: scroll_column.implicitHeight

                Column {
                    id: scroll_column
                    width: main_column.width
                    spacing: 10

                    AddCombatantRow {
                        id: row1
                        onCopyPrevious: {
                            setCombatant(name_field.text, ini_spinbox.value,
                                         health_spinbox.value)
                        }
                    }

                    AddCombatantRow {
                        id: row2
                        onCopyPrevious: {
                            setCombatant(row1.getName(), row1.getIni(),
                                         row1.getHealth())
                        }
                    }

                    AddCombatantRow {
                        id: row3
                        onCopyPrevious: {
                            setCombatant(row2.getName(), row2.getIni(),
                                         row2.getHealth())
                        }
                    }

                    AddCombatantRow {
                        id: row4
                        onCopyPrevious: {
                            setCombatant(row3.getName(), row3.getIni(),
                                         row3.getHealth())
                        }
                    }

                    AddCombatantRow {
                        id: row5
                        onCopyPrevious: {
                            setCombatant(row4.getName(), row4.getIni(),
                                         row4.getHealth())
                        }
                    }

                    AddCombatantRow {
                        id: row6
                        onCopyPrevious: {
                            setCombatant(row5.getName(), row5.getIni(),
                                         row5.getHealth())
                        }
                    }

                    AddCombatantRow {
                        id: row7
                        onCopyPrevious: {
                            setCombatant(row6.getName(), row6.getIni(),
                                         row6.getHealth())
                        }
                    }

                    AddCombatantRow {
                        id: row8
                        onCopyPrevious: {
                            setCombatant(row7.getName(), row7.getIni(),
                                         row7.getHealth())
                        }
                    }

                    AddCombatantRow {
                        id: row9
                        onCopyPrevious: {
                            setCombatant(row8.getName(), row8.getIni(),
                                         row8.getHealth())
                        }
                    }
                }
            }
        }
    }

    standardButtons: Dialog.Ok | Dialog.Cancel

    onAccepted: {
        // Add all characters
        console.log(addAdditionalCombatants)
        if (addAdditionalCombatants) {
            addCombatant(name_field.text, ini_spinbox.value,
                         health_spinbox.value, false)
            addCombatant(row1.getName(), row1.getIni(), row1.getHealth(), false)
            addCombatant(row2.getName(), row2.getIni(), row2.getHealth(), false)
            addCombatant(row3.getName(), row3.getIni(), row3.getHealth(), false)
            addCombatant(row4.getName(), row4.getIni(), row4.getHealth(), false)
            addCombatant(row5.getName(), row5.getIni(), row5.getHealth(), false)
            addCombatant(row6.getName(), row6.getIni(), row6.getHealth(), false)
            addCombatant(row7.getName(), row7.getIni(), row7.getHealth(), false)
            addCombatant(row8.getName(), row8.getIni(), row8.getHealth(), false)
            addCombatant(row9.getName(), row9.getIni(), row9.getHealth(), false)
        } else {
            addCombatant(name_field.text, ini_spinbox.value,
                         health_spinbox.value, false)
        }

        combat_tracker.sortByIni()

        // Clear all rows
        name_field.clear()
        ini_spinbox.value = 0
        health_spinbox.value = 0

        row1.clear()
        row2.clear()
        row3.clear()
        row4.clear()
        row5.clear()
        row6.clear()
        row7.clear()
        row8.clear()
        row9.clear()
    }
}
