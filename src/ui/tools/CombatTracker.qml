import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import gm.companion.combattracker 1.0
import gm.companion.colorscheme 1.0
import gm.companion.platforms 1.0

import "./combat_tracker"
import FontAwesome 2.0

Page {
    id: combat_tracker

    readonly property bool inPortrait: width < height
    property bool dice_enabled: false

    CombatTrackerTool {
        id: tool

        onCombatantsChanged: {
            tracker_model.clear()

            for (var i = 0; i < getListSize(); i++) {
                tracker_model.append({
                                         "name": getName(i),
                                         "ini": getIni(i),
                                         "health": getHealth(i),
                                         "status": getStatus(i),
                                         "notes": getNotes(i)
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

    Component.onCompleted: {
        combat_dice.setSource("Dice.qml", {
                                  "combat_tracker_mode": true
                              })
    }

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    header: top_bar
    footer: bottom_bar

    CombatantDialog {
        id: add_combatant_dialog

        onAddCombatant: {
            tool.add(name, ini, health, sort)
        }
    }

    Rectangle {
        id: top_bar
        height: color_scheme.toolbarHeight
        color: color_scheme.menuColor

        Row {
            id: top_row
            anchors.fill: parent

            padding: 10
            spacing: 10

            Button {
                hoverEnabled: true
                width: next_row.width

                background: Rectangle {
                    color: "transparent"
                }

                Row {
                    id: next_row
                    anchors.centerIn: parent
                    spacing: 10
                    padding: 10

                    Text {
                        text: FontAwesome.chevronRight
                        font.family: FontAwesome.familySolid
                        font.pointSize: 15
                        anchors.verticalCenter: parent.verticalCenter
                        color: parent.parent.pressed ? "grey" : parent.parent.hovered ? "lightgrey" : "white"
                    }

                    Text {
                        text: qsTr("Next")
                        anchors.verticalCenter: parent.verticalCenter
                        color: parent.parent.pressed ? "grey" : parent.parent.hovered ? "lightgrey" : "white"
                        font.bold: true
                        font.pointSize: 12
                    }
                }

                height: parent.height - parent.padding * 2
                anchors.verticalCenter: parent.verticalCenter
                onClicked: tool.next()
            }

            Button {
                hoverEnabled: true
                width: add_row.width

                background: Rectangle {
                    color: "transparent"
                }

                Row {
                    id: add_row
                    anchors.centerIn: parent
                    spacing: 10
                    padding: 10

                    Text {
                        text: FontAwesome.plus
                        font.family: FontAwesome.familySolid
                        font.pointSize: 15
                        anchors.verticalCenter: parent.verticalCenter
                        color: parent.parent.pressed ? "grey" : parent.parent.hovered ? "lightgrey" : "white"
                    }

                    Text {
                        text: qsTr("Add")
                        anchors.verticalCenter: parent.verticalCenter
                        color: parent.parent.pressed ? "grey" : parent.parent.hovered ? "lightgrey" : "white"
                        font.bold: true
                        font.pointSize: 12
                    }
                }

                height: parent.height - parent.padding * 2
                anchors.verticalCenter: parent.verticalCenter
                onClicked: add_combatant_dialog.open()
            }

            Button {
                hoverEnabled: true
                width: sort_row.width

                background: Rectangle {
                    color: "transparent"
                }

                Row {
                    id: sort_row
                    anchors.centerIn: parent
                    spacing: 10
                    padding: 10

                    Text {
                        text: FontAwesome.sort
                        font.family: FontAwesome.familySolid
                        font.pointSize: 15
                        anchors.verticalCenter: parent.verticalCenter
                        color: parent.parent.pressed ? "grey" : parent.parent.hovered ? "lightgrey" : "white"
                    }

                    Text {
                        text: qsTr("Sort by INI")
                        anchors.verticalCenter: parent.verticalCenter
                        color: parent.pressed ? "grey" : parent.parent.hovered ? "lightgrey" : "white"
                        font.bold: true
                        font.pointSize: 12
                    }
                }

                height: parent.height - parent.padding * 2
                anchors.verticalCenter: parent.verticalCenter
                onClicked: tool.sortByIni()
            }

            Button {
                hoverEnabled: true
                width: dice_row.width

                background: Rectangle {
                    color: "transparent"
                }

                Row {
                    id: dice_row
                    anchors.centerIn: parent
                    spacing: 10
                    padding: 10

                    Text {
                        text: FontAwesome.dice
                        font.family: FontAwesome.familySolid
                        font.pointSize: 15
                        anchors.verticalCenter: parent.verticalCenter
                        color: parent.parent.pressed ? "grey" : parent.parent.hovered ? "lightgrey" : "white"
                    }

                    Text {
                        id: dice_text
                        text: qsTr("Dice")
                        anchors.verticalCenter: parent.verticalCenter
                        color: parent.parent.pressed ? "grey" : parent.parent.hovered ? "lightgrey" : "white"
                        font.bold: true
                        font.pointSize: 12
                    }
                }

                height: parent.height - parent.padding * 2
                anchors.verticalCenter: parent.verticalCenter
                onClicked: dice_enabled ? dice_enabled = false : dice_enabled = true
            }
        }
    }

    // List Header
    Rectangle {
        id: list_header
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        height: top_bar.height

        color: color_scheme.listHeaderBackgroundColor

        Row {
            anchors.fill: parent
            spacing: 10
            padding: 10

            Text {
                text: qsTr("Name")
                anchors.verticalCenter: parent.verticalCenter
                width: list_view.width / 5
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 12
                color: color_scheme.listHeaderTextColor
            }

            Text {
                text: qsTr("Initiative")
                anchors.verticalCenter: parent.verticalCenter
                width: list_view.width / 6
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 12
                color: color_scheme.listHeaderTextColor
            }

            Text {
                text: qsTr("Health")
                anchors.verticalCenter: parent.verticalCenter
                width: list_view.width / 6
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 12
                color: color_scheme.listHeaderTextColor
            }

            Text {
                text: qsTr("Status")
                anchors.verticalCenter: parent.verticalCenter
                width: list_view.width / 6
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 12
                color: color_scheme.listHeaderTextColor
            }

            Text {
                text: qsTr("Notes")
                anchors.verticalCenter: parent.verticalCenter
                width: list_view.width / 6
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 12
                color: color_scheme.listHeaderTextColor
            }
        }
    }

    Flow {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: list_header.bottom
        anchors.bottom: parent.bottom

        // Combatant List
        ListView {
            id: list_view

            width: combat_dice.visible
                   && !inPortrait ? parent.width * 0.6 : parent.width
            height: parent.height

            clip: true
            spacing: 10

            ScrollBar.vertical: ScrollBar {
            }

            model: ListModel {
                id: tracker_model
            }

            delegate: Rectangle {
                height: delegate_row.height
                anchors.left: parent.left
                anchors.right: parent.right
                color: index == tool.currentIndex ? color_scheme.primaryButtonColor : "transparent"

                Row {
                    id: delegate_row
                    padding: 10
                    spacing: 10
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: color_scheme.toolbarHeight * 1.3

                    Text {
                        text: name
                        color: index == tool.currentIndex ? "white" : color_scheme.textColor
                        width: list_view.width / 5
                        clip: true
                        elide: Text.ElideRight
                        anchors.verticalCenter: parent.verticalCenter
                        font.pointSize: 12
                        font.bold: true
                    }

                    ListSpinBox {
                        field_value: ini
                        onValueChanged: tool.setIni(index, field_value)
                        text_color: index == tool.currentIndex ? "white" : color_scheme.textColor
                        current_item: index == tool.currentIndex ? true : false
                        from: 0
                        to: 1000
                    }

                    ListSpinBox {
                        field_value: health
                        onValueChanged: tool.setHealth(index, field_value)
                        text_color: index == tool.currentIndex ? "white" : color_scheme.textColor
                        current_item: index == tool.currentIndex ? true : false
                        from: 0
                        to: 1000
                    }

                    ListTextField {
                        onField_textChanged: tool.setStatus(index, field_text)
                        text_color: index == tool.currentIndex ? "white" : color_scheme.textColor
                        field_text: status
                    }

                    ListTextField {
                        onField_textChanged: tool.setNotes(index, field_text)
                        text_color: index == tool.currentIndex ? "white" : color_scheme.textColor
                        field_text: notes
                    }

                    Button {
                        id: delegate_remove_image
                        hoverEnabled: true
                        background: Rectangle {
                            color: "transparent"
                        }

                        Text {
                            text: FontAwesome.times
                            font.family: FontAwesome.familySolid
                            font.pointSize: 25
                            color: parent.pressed ? "grey" : (parent.hovered ? "lightgrey" : (index == tool.currentIndex ? "white" : color_scheme.primaryButtonColor))
                            anchors.centerIn: parent
                        }

                        height: (parent.height - parent.padding * 2)
                        width: height
                        anchors.verticalCenter: parent.verticalCenter

                        onClicked: tool.remove(index)
                    }
                }
            }
        }

        Loader {
            id: combat_dice

            width: parent.width - list_view.width
            height: parent.height

            visible: !inPortrait && dice_enabled
            asynchronous: true
        }
    }

    Rectangle {
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
                height: color_scheme.toolbarHeight
                anchors.left: parent.left
                anchors.right: parent.right
                color: color_scheme.menuColor

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

                        Text {
                            text: qsTr("Round:")
                            anchors.verticalCenter: parent.verticalCenter
                            color: "white"
                            font.bold: true
                            font.pointSize: 12
                        }

                        Text {
                            text: tool.currentRound
                            anchors.verticalCenter: parent.verticalCenter
                            color: "white"
                            font.bold: true
                            font.pointSize: 12
                        }
                    }

                    Button {
                        id: reset_button

                        hoverEnabled: true
                        width: reset_row.width

                        background: Rectangle {
                            color: "transparent"
                        }

                        Row {
                            id: reset_row
                            anchors.centerIn: parent
                            spacing: 10
                            padding: 10

                            Text {
                                text: FontAwesome.undo
                                font.family: FontAwesome.familySolid
                                font.pointSize: 14
                                anchors.verticalCenter: parent.verticalCenter
                                color: parent.parent.pressed ? "grey" : parent.parent.hovered ? "lightgrey" : "white"
                            }

                            Text {
                                text: qsTr("Reset")
                                anchors.verticalCenter: parent.verticalCenter
                                color: parent.parent.pressed ? "grey" : parent.parent.hovered ? "lightgrey" : "white"
                                font.bold: true
                                font.pointSize: 12
                            }
                        }

                        anchors.verticalCenter: parent.verticalCenter
                        height: parent.height - parent.padding * 2
                        onClicked: tool.resetRounds()
                    }

                    Button {
                        hoverEnabled: true
                        width: clear_row.width

                        background: Rectangle {
                            color: "transparent"
                        }

                        Row {
                            id: clear_row
                            anchors.centerIn: parent
                            spacing: 10
                            padding: 10

                            Text {
                                text: FontAwesome.times
                                font.family: FontAwesome.familySolid
                                font.pointSize: 17
                                anchors.verticalCenter: parent.verticalCenter
                                color: parent.parent.pressed ? "grey" : parent.parent.hovered ? "lightgrey" : "white"
                            }

                            Text {
                                text: qsTr("Clear")
                                anchors.verticalCenter: parent.verticalCenter
                                color: parent.parent.pressed ? "grey" : parent.parent.hovered ? "lightgrey" : "white"
                                font.bold: true
                                font.pointSize: 12
                            }
                        }

                        anchors.verticalCenter: parent.verticalCenter
                        height: parent.height - parent.padding * 2
                        onClicked: tool.clear()
                    }
                }
            }
        }
    }
}
