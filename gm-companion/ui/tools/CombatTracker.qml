import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3

import gm.companion.combattracker 1.0
import gm.companion.colorscheme 1.0
import gm.companion.platforms 1.0

import "./combat_tracker"

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

    Component.onCompleted: {
        combat_dice.setSource("Dice.qml", {
                                  combat_tracker_mode: true
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

            padding: 5
            leftPadding: 0
            spacing: 5

            Button {
                text: qsTr("Next")
                height: parent.height - parent.padding * 2
                anchors.verticalCenter: parent.verticalCenter
                onClicked: tool.next()
            }

            Button {
                text: qsTr("Add")
                height: parent.height - parent.padding * 2
                anchors.verticalCenter: parent.verticalCenter
                onClicked: add_combatant_dialog.open()
            }

            Button {
                text: qsTr("Sort by INI")
                height: parent.height - parent.padding * 2
                anchors.verticalCenter: parent.verticalCenter
                onClicked: tool.sortByIni()
            }

            Button {
                text: qsTr("Dice")
                height: parent.height - parent.padding * 2
                anchors.verticalCenter: parent.verticalCenter
                onClicked: dice_enabled ? dice_enabled = false : dice_enabled = true
            }
        }
    }

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
                        font.pointSize: 12
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
                        source: index == tool.currentIndex ? "/icons/menu/x_sign_bright.png" : "/icons/menu/x_sign_" + color_scheme.combatTrackerXType + ".png"
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

                    Rectangle {
                        height: parent.height - parent.padding * 2
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
                        height: parent.height - parent.padding * 2
                        onClicked: tool.resetRounds()
                    }

                    Button {
                        text: qsTr("Clear")
                        anchors.verticalCenter: parent.verticalCenter
                        height: parent.height - parent.padding * 2
                        onClicked: tool.clear()
                    }
                }
            }
        }
    }
}
