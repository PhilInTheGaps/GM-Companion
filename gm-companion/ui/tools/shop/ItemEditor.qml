import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4 as Controls1_4

import gm.companion.itemeditor 1.0

Page {
    id: editor

    signal backToShopEditor

    Component.onCompleted: editor_tool.updateCategories()

    ItemEditorTool {
        id: editor_tool

        onCategoriesChanged: category_combobox.model = categories
        onItemsChanged: loadItemTable()

        function loadItemTable() {
            table_model.clear()

            for (var i = 0; i < getItemNames().length; i++) {
                table_model.append({
                                       name: getItemNames()[i],
                                       price: getItemPrices()[i],
                                       category: getItemCategories()[i],
                                       description: getItemDescriptions()[i]
                                   })
            }
        }
    }

    Row {
        width: parent.width
        height: parent.height
        spacing: 5

        Column {
            id: item_properties_column
            width: (parent.width - parent.spacing) / 3
            height: parent.height
            spacing: 5

            Button {
                text: qsTr("Back")

                onClicked: backToShopEditor()
            }

            Text {
                text: qsTr("Add New Items")
            }

            Row {
                width: parent.width
                spacing: 5

                TextField {
                    id: item_name_field
                    width: (parent.width - parent.spacing * 2) / 3
                    placeholderText: qsTr("Item Name")
                }

                TextField {
                    id: item_price_field
                    width: (parent.width - parent.spacing * 2) / 3
                    placeholderText: qsTr("Price")
                }

                ComboBox {
                    id: category_combobox
                    width: (parent.width - parent.spacing * 2) / 3
                }
            }

            TextField {
                id: item_description_field
                width: parent.width
                placeholderText: qsTr("Item Description")
            }

            Button {
                text: qsTr("Add Item")

                onClicked: {
                    if (item_name_field.text != ""
                            && category_combobox.currentText != "") {
                        editor_tool.addItem(item_name_field.text,
                                            item_price_field.text,
                                            category_combobox.currentText,
                                            item_description_field.text)
                    }
                }
            }

            Text {
                text: qsTr("Add New Categories")
            }

            Row {
                width: parent.width
                spacing: 5

                Button {
                    id: add_category_button
                    text: qsTr("Add Category")

                    onClicked: {
                        if (category_name_field.text != "") {
                            editor_tool.addCategory(category_name_field.text)
                        }
                    }
                }

                TextField {
                    id: category_name_field
                    width: parent.width - parent.spacing - add_category_button.width
                    placeholderText: qsTr("Category Name")
                }
            }

            TextArea {
                width: parent.width
                padding: 0
                readOnly: true

                text: qsTr("Tutorial:\n
Categories
- To add a new item category enter a category name
and click \"Add Category\"

Items
- Items are organized in categories
- If you have not created any categories, read \"Categories\" above

- How to add a new Item:
- Enter item name in \"Item Name\" field
- Set a price the item sells at (not required)
- Choose a category from the combo box
- Enter a description in the \"Item Description\" field (not required)
- Click \"Add Item\"")
            }
        }

        Column {
            width: parent.width - parent.spacing - item_properties_column.width
            height: parent.height
            spacing: 5

            ListModel {
                id: table_model
            }

            Controls1_4.TableView {
                id: item_table_view
                width: parent.width
                height: parent.height - parent.spacing - delete_item_button.height

                model: table_model

                Component.onCompleted: editor_tool.updateItems()

                Controls1_4.TableViewColumn {
                    id: item_column
                    title: qsTr("Item")
                    role: "name"
                    movable: false
                    width: item_table_view.width / 6
                }

                Controls1_4.TableViewColumn {
                    id: price_column
                    title: qsTr("Price")
                    role: "price"
                    movable: false
                    width: item_table_view.width / 6
                }

                Controls1_4.TableViewColumn {
                    id: category_column
                    title: qsTr("Category")
                    role: "category"
                    movable: false
                    width: item_table_view.width / 6
                }

                Controls1_4.TableViewColumn {
                    id: description_column
                    title: qsTr("Description")
                    role: "description"
                    movable: false
                    width: item_table_view.width - item_column.width
                           - price_column.width - category_column.width - 2
                }
            }

            Button {
                id: delete_item_button
                text: qsTr("Delete Selected")

                onClicked: editor_tool.deleteItem(item_table_view.currentRow)
            }
        }
    }
}
