import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import gm.companion.defaultsheet 1.0
import gm.companion.charactersaveload 1.0
import "./sheet_elements"

Page {
    id: sheet_page

    property string character_name: ""

    function save() {
        save_load.initializeSaving()
    }

    function load() {
        save_load.initializeLoading()
    }

    DefaultSheetTool {
        id: sheet_tool

        character_name: sheet_page.character_name
    }

    CharacterSaveLoad {
        id: save_load

        onSaving: swipe_view.save()

        onLoading: swipe_view.load()
    }

    Column {
        anchors.fill: parent
        spacing: 5

        Flow {
            id: page_control_flow
            spacing: 5
            width: parent.width

            Button {
                text: qsTr("Info")

                onClicked: swipe_view.setCurrentIndex(0)
            }

            Button {
                text: qsTr("Abilities")

                onClicked: swipe_view.setCurrentIndex(1)
            }

            Button {
                text: qsTr("Inventory")

                onClicked: swipe_view.setCurrentIndex(2)
            }
        }

        SwipeView {
            id: swipe_view
            clip: true
            width: parent.width
            height: parent.height - page_control_flow.height - parent.spacing

            function save() {
                info_table.character_name = sheet_page.character_name
                info_table.save()

                attributes_table.character_name = sheet_page.character_name
                attributes_table.save()

                abilities1_table.character_name = sheet_page.character_name
                abilities1_table.save()

                abilities2_table.character_name = sheet_page.character_name
                abilities2_table.save()

                inventory_table.character_name = sheet_page.character_name
                inventory_table.save()

                save_load.saveText(sheet_page.character_name, "info", "bio",
                                   bio_text_area.text)
            }

            function load() {
                info_table.col1_list = save_load.loadTable(
                            sheet_tool.character_name, "info", 0)
                info_table.col2_list = save_load.loadTable(
                            sheet_tool.character_name, "info", 1)
                info_table.fillList()

                attributes_table.col1_list = save_load.loadTable(
                            sheet_tool.character_name, "attributes", 0)
                attributes_table.col2_list = save_load.loadTable(
                            sheet_tool.character_name, "attributes", 1)
                attributes_table.fillList()

                abilities1_table.col1_list = save_load.loadTable(
                            sheet_tool.character_name, "abilities1", 0)
                abilities1_table.col2_list = save_load.loadTable(
                            sheet_tool.character_name, "abilities1", 1)
                abilities1_table.fillList()

                abilities2_table.col1_list = save_load.loadTable(
                            sheet_tool.character_name, "abilities2", 0)
                abilities2_table.col2_list = save_load.loadTable(
                            sheet_tool.character_name, "abilities2", 1)
                abilities2_table.fillList()

                inventory_table.col1_list = save_load.loadTable(
                            sheet_tool.character_name, "inventory", 0)
                inventory_table.col2_list = save_load.loadTable(
                            sheet_tool.character_name, "inventory", 1)
                inventory_table.fillList()

                bio_text_area.text = save_load.loadText(
                            sheet_page.character_name, "info", "bio")
            }

            Row {
                spacing: 5

                EditableTable {
                    id: info_table
                    width: (parent.width - parent.spacing) / 2 - parent.leftPadding
                    height: parent.height - parent.topPadding * 2

                    character_name: sheet_tool.character_name
                    table_name: "info"

                    columns_visible: false
                    col1_list: ["Character", "Player", "Level", "Home", "Location", "Race", "Size", "Gender", "Age", "Height", "Weight", "Hair Color", "Eye Color"]
                    col2_list: [sheet_tool.character_name, sheet_tool.player_name]
                }

                Column {
                    width: (parent.width - parent.spacing) / 2 - parent.rightPadding
                    height: parent.height - parent.topPadding * 2
                    spacing: 5

                    Text {
                        id: bio_text
                        text: qsTr("Character Biography")
                    }

                    TextArea {
                        id: bio_text_area
                        width: parent.width - parent.leftPadding * 2
                        height: parent.height - bio_text.height - parent.spacing
                    }
                }
            }

            Row {
                //                anchors.fill: parent
                spacing: 5

                EditableTable {
                    id: attributes_table
                    width: (parent.width - parent.spacing * 2 - parent.leftPadding * 2) / 3
                    height: parent.height - parent.padding * 2

                    col1_name: "Attribute"
                    col2_name: "Value"
                    col1_width_divisor: 2
                    row_count: 8

                    character_name: sheet_tool.character_name
                    table_name: "attributes"
                }

                EditableTable {
                    id: abilities1_table
                    width: (parent.width - parent.spacing * 2 - parent.leftPadding * 2) / 3
                    height: parent.height - parent.padding * 2

                    col1_name: "Ability"
                    col2_name: "Value"
                    row_count: 25
                    col1_width_divisor: 2

                    character_name: sheet_tool.character_name
                    table_name: "abilities1"
                }

                EditableTable {
                    id: abilities2_table
                    width: (parent.width - parent.spacing * 2 - parent.leftPadding * 2) / 3
                    height: parent.height - parent.padding * 2

                    col1_name: "Ability"
                    col2_name: "Value"
                    row_count: 25
                    col1_width_divisor: 2

                    character_name: sheet_tool.character_name
                    table_name: "abilities2"
                }
            }

            Column {
                spacing: 5

                EditableTable {
                    id: inventory_table
                    width: parent.width - padding * 2
                    height: parent.height - padding * 2

                    col1_name: "Item"
                    col2_name: "Weight"

                    col1_width_divisor: 5 / 4
                    row_count: 25

                    character_name: sheet_tool.character_name
                    table_name: "inventory"
                }
            }
        }
    }
}
