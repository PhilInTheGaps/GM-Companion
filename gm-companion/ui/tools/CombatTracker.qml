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
                    placeholderText: qsTr("Name")
                    selectByMouse: true
                }

                SpinBox {
                    id: add_ini_spinbox
                    width: dialog_column.width / 4
                    editable: true
                }

                SpinBox {
                    id: add_health_spinbox
                    width: dialog_column.width / 4
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

    Rectangle {
        id: top_bar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        height: parent.height / 12
        color: color_scheme.menuColor

        Row {
            anchors.fill: parent
            padding: 5
            spacing: 5

            Button {
                text: qsTr("Next")
                anchors.verticalCenter: parent.verticalCenter
            }

            Button {
                text: qsTr("Add")
                anchors.verticalCenter: parent.verticalCenter
            }

            Button {
                text: qsTr("Dice")
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    Rectangle {
        id: list_header
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: top_bar.bottom

        height: parent.height / 15

        Row {
            anchors.fill: parent
            spacing: 5
            padding: 5

            Text {
                text: qsTr("Name")
                anchors.verticalCenter: parent.verticalCenter
                width: list_view.width / 5
            }

            Text {
                text: qsTr("Initiative")
                anchors.verticalCenter: parent.verticalCenter
                width: list_view.width / 6
            }

            Text {
                text: qsTr("Health")
                anchors.verticalCenter: parent.verticalCenter
                width: list_view.width / 6
            }

            Text {
                text: qsTr("Status")
                anchors.verticalCenter: parent.verticalCenter
                width: list_view.width / 6
            }

            Text {
                text: qsTr("Notes")
                anchors.verticalCenter: parent.verticalCenter
                width: list_view.width / 6
            }
        }
    }

    ListView {
        id: list_view
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: list_header.bottom
        anchors.bottom: bottom_bar.top

        clip: true
        spacing: 5

        model: ListModel {
            id: tracker_model

            ListElement {
                name: "Frank Lightspeed"
                ini: 12
                health: 25
                status: "Alive"
                notes: "Is super cool"
            }

            ListElement {
                name: "Berythag Appelfass"
                ini: 10
            }

            ListElement {
                name: "Test"
                ini: 12
            }

            ListElement {
                name: "Test"
                ini: 12
            }
        }

        delegate: Rectangle {
            height: delegate_row.height
            width: list_view.width

            Row {
                id: delegate_row
                padding: 5
                spacing: 5

                Text {
                    text: name
                    width: list_view.width / 5
                    clip: true
                    elide: Text.ElideRight
                    anchors.verticalCenter: parent.verticalCenter
                }

                SpinBox {
                    value: ini
                    width: list_view.width / 6
                    editable: true
                }

                SpinBox {
                    value: health
                    width: list_view.width / 6
                    editable: true
                }

                TextField {
                    text: status
                    width: list_view.width / 6
                    selectByMouse: true
                }

                TextField {
                    text: notes
                    width: list_view.width / 6
                    selectByMouse: true
                }

                Image {
                    id: delegate_remove_image
                    source: "/icons/menu/x_sign_dark.png"
                    height: parent.height - parent.padding * 2
                    width: height
                }
            }
        }
    }

    Rectangle {
        id: bottom_bar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        height: parent.height / 12
        color: color_scheme.menuColor

        Row {
            anchors.fill: parent
            padding: 5
            spacing: 5

            Rectangle {
                height: reset_button.height
                width: 200
                color: "white"
                anchors.verticalCenter: parent.verticalCenter

                Row {
                    spacing: 5
                    padding: 5
                    anchors.verticalCenter: parent.verticalCenter

                    Text {
                        text: qsTr("Round:")
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Text {
                        text: combat_tracker_tool.currentRound
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }

            Button {
                id: reset_button
                text: qsTr("Reset")
                anchors.verticalCenter: parent.verticalCenter
            }

            Button {
                text: qsTr("Clear")
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    //    Dice {
    //        id: combat_dice
    //        width: inPortrait ? parent.width - parent.padding
    //                            * 2 : (parent.width - parent.padding * 2 - parent.spacing) / 2
    //        height: inPortrait ? main_column.height : parent.height - parent.padding * 2
    //        visible: false
    //    }
}
