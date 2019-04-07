import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import "./combat_tracker"
import "./dice"
import FontAwesome 2.0

Page {
    id: root

    readonly property bool inPortrait: width < height || width < 1200
    property bool dice_enabled: false

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    header: top_bar
    footer: bottom_bar

    // Top Bar
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
                onClicked: combat_tracker.next()
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
                onClicked: {
                    add_rect.visible = !add_rect.visible
                    name_textfield.forceActiveFocus()
                }
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

    // Main Item
    Item {
        anchors.fill: parent

        // List Header
        Rectangle {
            id: list_header
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: top_bar.height

            color: color_scheme.listHeaderBackgroundColor

            Text {
                id: name_header
                text: qsTr("Name")
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 10
                width: list_view.width / 5
                horizontalAlignment: Text.AlignLeft
                font.pointSize: 12
                color: color_scheme.listHeaderTextColor
            }

            Text {
                id: ini_header
                text: qsTr("Initiative")
                anchors.left: name_header.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 10
                width: list_view.width / 6
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 12
                color: color_scheme.listHeaderTextColor
            }

            Text {
                id: health_header
                text: qsTr("Health")
                anchors.left: ini_header.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 10
                width: list_view.width / 6
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 12
                color: color_scheme.listHeaderTextColor
            }

            Text {
                id: notes_header
                text: qsTr("Notes")
                anchors.left: health_header.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 10
                width: dice_label.visible ? parent.width - combat_dice.width - x
                                            - 10 - color_scheme.toolbarHeight
                                            * 1.3 - 20 : parent.width - x - 10
                                            - color_scheme.toolbarHeight * 1.3 - 20
                horizontalAlignment: Text.AlignLeft
                font.pointSize: 12
                color: color_scheme.listHeaderTextColor
            }

            Text {
                id: dice_label
                text: qsTr("Dice")
                anchors.verticalCenter: parent.verticalCenter
                x: combat_dice.x + 10
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 12
                color: color_scheme.listHeaderTextColor
                visible: dice_enabled && !inPortrait
            }
        }

        Rectangle {
            id: add_rect
            visible: false
            height: color_scheme.toolbarHeight * 1.3
            anchors.top: list_header.bottom
            anchors.left: parent.left
            anchors.right: dice_enabled
                           && !inPortrait ? combat_dice.left : parent.right

            color: color_scheme.backgroundColor

            function addCombatants() {
                if (name_textfield.text != "") {
                    combat_tracker.add(name_textfield.text, ini_spinbox.value,
                                       health_spinbox.value,
                                       notes_textfield.text)
                }
            }

            function clearAddFields() {
                name_textfield.clear()
                ini_spinbox.value = 0
                health_spinbox.value = 0
                notes_textfield.clear()
            }

            Item {
                anchors.fill: parent
                anchors.margins: 10

                Row {
                    anchors.left: parent.left
                    anchors.right: add_button.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: 10
                    spacing: 10

                    TextField {
                        id: name_textfield
                        width: list_view.width / 5
                        selectByMouse: true

                        placeholderText: qsTr("Name")

                        onAccepted: {
                            add_rect.addCombatants()
                            add_rect.clearAddFields()
                        }
                    }

                    SpinBox {
                        id: ini_spinbox
                        width: list_view.width / 6
                        from: 0
                        to: 99999
                        editable: true
                    }

                    SpinBox {
                        id: health_spinbox
                        width: list_view.width / 6
                        from: 0
                        to: 99999
                        editable: true
                    }

                    TextField {
                        id: notes_textfield
                        width: parent.width - x

                        onAccepted: {
                            add_rect.addCombatants()
                            add_rect.clearAddFields()
                            name_textfield.forceActiveFocus()
                        }
                    }
                }

                Button {
                    id: add_button

                    hoverEnabled: true
                    background: Rectangle {
                        color: parent.pressed ? "lightgrey" : "white"
                    }

                    Text {
                        text: FontAwesome.plus
                        font.family: FontAwesome.familySolid
                        font.pixelSize: parent.height / 2
                        color: "black"
                        anchors.centerIn: parent
                    }

                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    width: height

                    onClicked: {
                        add_rect.addCombatants()
                        add_rect.clearAddFields()
                        add_rect.visible = false
                    }
                }
            }
        }

        // Combatant List
        ListView {
            id: list_view

            anchors.left: parent.left
            anchors.top: add_rect.visible ? add_rect.bottom : list_header.bottom
            anchors.bottom: parent.bottom
            anchors.right: dice_enabled
                           && !inPortrait ? undefined : parent.right
            width: dice_enabled && !inPortrait ? parent.width * 0.65 : 0

            clip: true
            spacing: 10

            currentIndex: combat_tracker.currentIndex
            ScrollBar.vertical: ScrollBar {
            }
            model: combatantListModel

            delegate: Rectangle {
                id: delegate_root
                height: delegate_row.height
                anchors.left: parent.left
                anchors.right: parent.right
                color: ListView.isCurrentItem ? color_scheme.primaryButtonColor : "transparent"

                Row {
                    id: delegate_row
                    padding: 10
                    spacing: 10
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: color_scheme.toolbarHeight * 1.3

                    // Name
                    Text {
                        text: modelData.name
                        color: delegate_root.ListView.isCurrentItem ? "white" : color_scheme.textColor
                        width: list_view.width / 5
                        clip: true
                        elide: Text.ElideRight
                        anchors.verticalCenter: parent.verticalCenter
                        font.pointSize: 12
                        font.bold: true
                    }

                    // INI
                    ListSpinBox {
                        value: modelData.ini
                        width: list_view.width / 6
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        font_color: delegate_root.ListView.isCurrentItem ? "white" : color_scheme.textColor
                        onValueEdited: combat_tracker.setIni(index, new_value)
                        onValueIncreased: combat_tracker.modifyIni(index, steps)
                    }

                    // Health
                    ListSpinBox {
                        width: list_view.width / 6
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        value: modelData.health
                        font_color: delegate_root.ListView.isCurrentItem ? "white" : color_scheme.textColor
                        onValueEdited: combat_tracker.setHealth(index,
                                                                new_value)
                        onValueIncreased: combat_tracker.modifyHealth(index,
                                                                      steps)
                    }

                    // Notes
                    ListTextField {
                        width: parent.width - x - 20 - delegate_button_row.width
                        onField_textChanged: combat_tracker.setNotes(index,
                                                                     field_text)
                        text_color: delegate_root.ListView.isCurrentItem ? "white" : color_scheme.textColor
                        field_text: modelData.notes
                    }

                    Row {
                        id: delegate_button_row
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.margins: parent.padding

                        Button {
                            id: delegate_delay_button
                            hoverEnabled: true
                            background: Rectangle {
                                color: "transparent"
                            }

                            visible: !modelData.delay
                                     && delegate_root.ListView.isCurrentItem

                            Text {
                                text: FontAwesome.clock
                                font.family: FontAwesome.familySolid
                                font.pointSize: 20
                                color: parent.pressed ? "grey" : (parent.hovered ? "lightgrey" : "white")
                                anchors.centerIn: parent
                            }

                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            width: height

                            onClicked: combat_tracker.delayTurn(index)
                        }

                        Button {
                            id: delegate_remove_button
                            hoverEnabled: true
                            background: Rectangle {
                                color: "transparent"
                            }

                            Text {
                                text: FontAwesome.times
                                font.family: FontAwesome.familySolid
                                font.pointSize: 25
                                color: parent.pressed ? "grey" : (parent.hovered ? "lightgrey" : (delegate_root.ListView.isCurrentItem ? "white" : color_scheme.primaryButtonColor))
                                anchors.centerIn: parent
                            }

                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            width: height

                            onClicked: combat_tracker.remove(index)
                        }
                    }
                }
            }
        }

        Loader {
            id: combat_dice
            source: "dice/DiceCombatTracker.qml"

            anchors.left: list_view.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
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
                            text: combat_tracker.currentRound
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
                        onClicked: combat_tracker.resetRounds()
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
                        onClicked: combat_tracker.clear()
                    }
                }
            }
        }
    }
}
