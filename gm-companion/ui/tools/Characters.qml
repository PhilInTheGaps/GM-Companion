import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4

import gm.companion.charactertool 1.0
import gm.companion.colorscheme 1.0
import "./characters"

Page {
    id: characters

    CharacterTool {
        id: character_tool

        // Load character data
        function loadCharacter(character_name) {
            swipe_view.currentIndex = getSheetIndex(getSheetTemplate(
                                                        character_name))

            swipe_view.setCharacterName(character_name)

            swipe_view.load()
        }

        // Set a character active or inactive
        function toggleActive(character_name, active) {
            if (active) {
                setCharacterInactive(character_name)
            } else {
                setCharacterActive(character_name)
            }
        }

        // Load all active Characters
        function loadActiveCharacterList() {
            var character_list = getActiveCharacterList()

            for (var i = active_column.children.length - 1; i > -1; i--) {
                active_column.children[i].destroy()
            }

            var component = Qt.createComponent(
                        "./characters/CharacterButton.qml")

            for (var i = 0; i < character_list.length; i++) {
                var button = component.createObject(active_column, {
                                                        character_name: character_list[i]
                                                    })
                button.clicked.connect(loadCharacter)
                button.toggle_active_clicked.connect(toggleActive)
            }
        }

        // Load all inactive Characters
        function loadInactiveCharacterList() {
            var character_list = getInactiveCharacterList()

            for (var i = active_column.children.length - 1; i > -1; i--) {
                active_column.children[i].destroy()
            }

            var component = Qt.createComponent(
                        "./characters/CharacterButton.qml")

            for (var i = 0; i < character_list.length; i++) {
                var button = component.createObject(active_column, {
                                                        character_name: character_list[i],
                                                        active: false
                                                    })
                button.clicked.connect(loadCharacter)
                button.toggle_active_clicked.connect(toggleActive)
            }
        }

        onCharactersUpdated: loadActiveCharacterList()
    }

    ColorScheme {
        id: color_scheme
    }

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    Row {
        anchors.fill: parent
        spacing: 5
        padding: 5

        Column {
            id: left_column
            height: parent.height
            width: 150
            spacing: 5

            TextField {
                id: character_name_field
                width: parent.width
                placeholderText: qsTr("Character Name")
                selectByMouse: true
            }

            TextField {
                id: player_name_field
                width: parent.width
                placeholderText: qsTr("Player Name")
                selectByMouse: true
            }

            ComboBox {
                id: sheet_type_combobox
                width: parent.width
                model: ["Default", "DSA5"]
            }

            Button {
                id: new_character_button
                width: parent.width
                text: qsTr("Create new Character")

                onClicked: {
                    character_tool.addCharacter(
                                sheet_type_combobox.currentText,
                                character_name_field.text,
                                player_name_field.text)

                    swipe_view.currentIndex = character_tool.getSheetIndex(
                                character_tool.getSheetTemplate(
                                    sheet_type_combobox.currentText))

                    swipe_view.setCharacterName(sheet_type_combobox.currentText)

                    swipe_view.save()
                }
            }

            Column {
                width: parent.width
                height: parent.height - character_name_field.height
                        - player_name_field.height - sheet_type_combobox.height
                        - new_character_button.height - parent.spacing * 7

                spacing: 5

                Row {
                    id: active_inactive_row
                    width: parent.width
                    spacing: 5

                    Button {
                        text: qsTr("Active")
                        width: (parent.width - parent.spacing) / 2

                        onClicked: character_tool.loadActiveCharacterList()
                    }

                    Button {
                        text: qsTr("Inactive")
                        width: (parent.width - parent.spacing) / 2

                        onClicked: character_tool.loadInactiveCharacterList()
                    }
                }

                ScrollView {
                    id: character_scrollview
                    width: parent.width
                    height: parent.height - active_inactive_row.height - parent.spacing
                    clip: true

                    Column {
                        id: active_column
                        width: character_scrollview.width
                        spacing: 5

                        Component.onCompleted: {
                            character_tool.loadActiveCharacterList()
                        }
                    }
                }
            }
        }

        Column {
            width: (parent.width - left_column.width - parent.spacing - parent.leftPadding * 2)
            height: parent.height - parent.topPadding * 2
            spacing: 5

            Row {
                id: save_delete_row
                width: parent.width
                spacing: 5

                Button {
                    text: qsTr("Save Character")

                    onClicked: swipe_view.save()
                }

                DelayButton {
                    text: qsTr("Delete Character")
                    delay: 1200

                    onActivated: {
                        character_tool.deleteCharacter(
                                    swipe_view.getCharacterName())

                        swipe_view.setCurrentIndex(0)
                        swipe_view.setCharacterName("Unknown Character")
                    }
                }
            }

            SwipeView {
                id: swipe_view
                width: parent.width
                height: parent.height - parent.spacing - save_delete_row.height
                clip: true

                background: Rectangle {
                    color: color_scheme.backgroundColor
                }

                interactive: false
                currentIndex: 0

                function setCharacterName(character_name) {
                    switch (currentIndex) {
                    case 1:
                        default_sheet.character_name = character_name
                        break
                    case 2:
                        dsa5_sheet.character_name = character_name
                        break
                    default:
                        break
                    }
                }

                function getCharacterName() {
                    switch (currentIndex) {
                    case 1:
                        return default_sheet.character_name
                    case 2:
                        return dsa5_sheet.character_name
                    default:
                        return "Unknown Character"
                    }
                }

                function save() {
                    switch (currentIndex) {
                    case 1:
                        default_sheet.save()
                        break
                    case 2:
                        dsa5_sheet.save()
                        break
                    default:
                        break
                    }
                }

                function load() {
                    switch (currentIndex) {
                    case 1:
                        default_sheet.load()
                        break
                    case 2:
                        dsa5_sheet.load()
                        break
                    default:
                        break
                    }
                }

                Rectangle {
                    id: no_character_selected
                    color: color_scheme.backgroundColor

                    Text {
                        text: qsTr("No Character Selected")
                        color: color_scheme.textColor

                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }

                DefaultSheet {
                    id: default_sheet
                }

                DSA5Sheet {
                    id: dsa5_sheet
                }
            }
        }
    }
}
