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
        id: tool

        onCombatantsChanged: {
            tracker_model.clear()

            for (var i = 0; i < getListSize(); i++) {
                tracker_model.append({
                                         name: getName(i),
                                         ini: getIni(i),
                                         health: getHealth(i),
                                         status: getStatus(i),
                                         notes: getNotes(i)
                                     })
            }
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
        modal: true

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
            tool.add(add_name_field.text, add_ini_spinbox.value,
                     add_health_spinbox.value)
        }
    }

    Rectangle {
        id: top_bar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        height: top_row.height
        color: color_scheme.menuColor

        Row {
            id: top_row
            anchors.left: parent.left
            anchors.right: parent.right

            padding: 5
            leftPadding: 0
            spacing: 5

            Button {
                text: qsTr("Next")
                anchors.verticalCenter: parent.verticalCenter
                onClicked: tool.next()
            }

            Button {
                text: qsTr("Add")
                anchors.verticalCenter: parent.verticalCenter
                onClicked: add_combatant_dialog.open()
            }

            Button {
                text: qsTr("Dice")
                anchors.verticalCenter: parent.verticalCenter
                onClicked: {
                    combat_dice.visible ? combat_dice.visible = false : combat_dice.visible = true
                }
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
            spacing: 10
            padding: 10

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

    Flow {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: list_header.bottom
        anchors.bottom: bottom_bar.top

        ListView {
            id: list_view

            width: combat_dice.visible
                   && !inPortrait ? parent.width * 0.6 : parent.width
            height: combat_dice.visible
                    && inPortrait ? parent.height / 2 : parent.height

            clip: true
            spacing: 10

            model: ListModel {
                id: tracker_model
            }

            delegate: Rectangle {
                height: delegate_row.height
                width: list_view.width
                color: index == tool.currentIndex ? color_scheme.primaryButtonColor : "transparent"

                Row {
                    id: delegate_row
                    padding: 10
                    spacing: 10

                    Text {
                        text: name
                        color: index == tool.currentIndex ? "white" : color_scheme.textColor
                        width: list_view.width / 5
                        clip: true
                        elide: Text.ElideRight
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    SpinBox {
                        value: ini
                        width: list_view.width / 6
                        editable: true
                        onValueChanged: tool.setIni(index, value)
                    }

                    SpinBox {
                        value: health
                        width: list_view.width / 6
                        editable: true
                        onValueChanged: tool.setHealth(index, value)
                    }

                    TextField {
                        text: status
                        width: list_view.width / 6
                        selectByMouse: true
                        onTextEdited: tool.setStatus(index, text)
                    }

                    TextField {
                        text: notes
                        width: list_view.width / 6
                        selectByMouse: true
                        onTextEdited: tool.setNotes(index, text)
                    }

                    Image {
                        id: delegate_remove_image
                        source: "/icons/menu/x_sign_dark.png"
                        height: (parent.height - parent.padding * 2) * 0.8
                        width: height
                        anchors.verticalCenter: parent.verticalCenter

                        MouseArea {
                            anchors.fill: parent
                            onClicked: tool.remove(index)
                        }
                    }
                }
            }
        }

        Dice {
            id: combat_dice
            width: inPortrait ? parent.width : parent.width - list_view.width
            height: inPortrait ? parent.height - list_view.height : parent.height
            visible: false
        }
    }

    Rectangle {
        id: bottom_bar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        height: bottom_row.height
        color: color_scheme.menuColor

        Row {
            id: bottom_row
            anchors.left: parent.left
            anchors.right: parent.right

            padding: 5
            leftPadding: 0
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
                        text: tool.currentRound
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }

            Button {
                id: reset_button
                text: qsTr("Reset")
                anchors.verticalCenter: parent.verticalCenter
                onClicked: tool.resetRounds()
            }

            Button {
                text: qsTr("Clear")
                anchors.verticalCenter: parent.verticalCenter
                onClicked: tool.clear()
            }
        }
    }
}
