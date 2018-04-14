import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4 as Controls1_4

import gm.companion.shoptool 1.0
import gm.companion.shopeditor 1.0

Page {
    id: editor

    signal backToViewer
    signal switchToItemEditor

    ShopTool {
        id: shop_tool
    }

    ShopEditorTool {
        id: editor_tool

        function updateShopFlow() {
            updateShopList()

            shop_flow.children = []

            var component = Qt.createComponent("./ShopButton.qml")

            console.log(getShopList())

            for (var i = 0; i < getShopList().length; i++) {

                console.log(getShopList()[i])

                var button = component.createObject(shop_flow, {
                                                        x: 0,
                                                        y: 0,
                                                        shop: getShopList()[i]
                                                    })

                button.clicked.connect(updateShopInformation)
            }
        }

        function updateShopInformation(shop) {
            loadShop(shop)

            shop_name_text.text = getShopName()
            shop_owner_textfield.text = getShopOwner()
            shop_description_textfield.text = getShopDescription()

            fillItemTable()
        }

        function fillItemTable() {
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

        function fillItemListTable() {
            item_list_model.clear()

            for (var i = 0; i < getItemListNames().length; i++) {
                item_list_model.append({
                                           name: getItemListNames()[i],
                                           price: getItemListPrices()[i],
                                           category: getItemListCategories()[i],
                                           description: getItemListDescriptions(
                                                            )[i]
                                       })
            }
        }

        function addListItem() {
            var index = item_list_table.currentRow

            addItem(getListItem(index))

            fillItemTable()
        }
    }

    Row {
        width: parent.width
        height: parent.height
        spacing: 5

        Column {
            id: shop_properties_column
            width: parent.width / 8
            height: parent.height
            spacing: 5

            Button {
                text: qsTr("Back")
                width: parent.width
                onClicked: backToViewer()
            }

            Text {
                text: qsTr("Projects")
            }

            ComboBox {
                id: project_combo_box
                width: parent.width
                model: shop_tool.projects

                onCurrentTextChanged: {
                    if (currentText != "") {
                        editor_tool.setCurrentProject(currentText)

                        category_combo_box.model = editor_tool.getCategories()
                    }
                }
            }

            Text {
                text: qsTr("Category")
            }

            ComboBox {
                id: category_combo_box
                width: parent.width
                Component.onCompleted: {
                    model = editor_tool.getCategories()
                }

                onCurrentTextChanged: {
                    if (currentText != "") {
                        editor_tool.setCurrentCategory(currentText)

                        editor_tool.updateShopFlow()
                    }
                }
            }

            TextField {
                id: project_textfield
                width: parent.width
                placeholderText: qsTr("Project Name")
                selectByMouse: true
            }

            Button {
                width: parent.width
                text: qsTr("Create New Project")

                onClicked: {
                    editor_tool.createProject(project_textfield.text)

                    project_combo_box.model = shop_tool.projects
                }
            }

            Text {
                text: qsTr("New Category")
            }

            TextField {
                id: category_textfield
                width: parent.width
                placeholderText: qsTr("Category Name")
                selectByMouse: true
            }

            Button {
                width: parent.width
                text: qsTr("Create New Category")

                onClicked: {
                    editor_tool.createCategory(category_textfield.text)

                    category_combo_box.model = editor_tool.getCategories()

                    category_combo_box.currentIndex = editor_tool.getCategories(
                                ).length - 1
                }
            }

            Text {
                text: qsTr("New Shop")
            }

            TextField {
                id: shop_name_textfield
                width: parent.width
                placeholderText: qsTr("Shop Name")
                selectByMouse: true
            }

            Button {
                width: parent.width
                text: qsTr("Create New Shop")

                onClicked: {
                    editor_tool.createShop(shop_name_textfield.text)

                    editor_tool.updateShopFlow()
                }
            }

            DelayButton {
                width: parent.width
                text: qsTr("Delete Shop")
                delay: 1200

                onActivated: {
                    editor_tool.deleteShop()

                    editor_tool.updateShopFlow()
                }
            }
        }

        Column {
            width: parent.width - parent.spacing - shop_properties_column.width
            height: parent.height
            spacing: 5

            Flow {
                id: shop_flow
                width: parent.width
                spacing: 5
            }

            Row {
                width: parent.width
                height: parent.height - parent.spacing - shop_flow.height
                spacing: 5

                Column {
                    width: (parent.width - parent.spacing) / 2
                    height: parent.height
                    spacing: 5

                    Row {
                        id: shop_properties_row
                        width: parent.width
                        spacing: 5

                        Text {
                            id: shop_name_text
                            text: qsTr("No Shop Selected")
                            anchors.verticalCenter: parent.verticalCenter
                            font.pointSize: 20

                            width: parent.width - parent.spacing - shop_owner_textfield.width
                            clip: true
                            elide: Text.ElideRight
                        }

                        TextField {
                            id: shop_owner_textfield
                            width: parent.width / 4
                            placeholderText: qsTr("Shop Owner")
                            selectByMouse: true

                            onTextEdited: editor_tool.setShopOwner(text)
                        }
                    }

                    TextField {
                        id: shop_description_textfield
                        width: parent.width
                        placeholderText: qsTr("Shop Description")
                        selectByMouse: true

                        onTextEdited: editor_tool.setShopDescription(text)
                    }

                    Controls1_4.TableView {
                        id: shop_items_table
                        width: parent.width
                        height: parent.height - parent.spacing * 3 - shop_properties_row.height
                                - shop_description_textfield.height - shop_action_row.height

                        model: ListModel {
                            id: table_model
                        }

                        Controls1_4.TableViewColumn {
                            id: item_column
                            title: qsTr("Item")
                            role: "name"
                            movable: false
                            width: shop_items_table.width / 6
                        }

                        Controls1_4.TableViewColumn {
                            id: price_column
                            title: qsTr("Price")
                            role: "price"
                            movable: false
                            width: shop_items_table.width / 6
                        }

                        Controls1_4.TableViewColumn {
                            id: category_column
                            title: qsTr("Category")
                            role: "category"
                            movable: false
                            width: shop_items_table.width / 6
                        }

                        Controls1_4.TableViewColumn {
                            id: description_column
                            title: qsTr("Description")
                            role: "description"
                            movable: false
                            width: shop_items_table.width - item_column.width
                                   - price_column.width - category_column.width - 2
                        }
                    }

                    Row {
                        id: shop_action_row
                        width: parent.width
                        spacing: 5

                        Button {
                            text: qsTr("Save Shop")

                            onClicked: editor_tool.saveShop()
                        }

                        Button {
                            text: qsTr("Remove Selected Item")

                            onClicked: {
                                var tableIndex = shop_items_table.currentRow

                                if (tableIndex > -1) {
                                    editor_tool.removeItem(tableIndex)

                                    editor_tool.fillItemTable()

                                    // Make sure next index is a legitimate index
                                    if (tableIndex >= shop_items_table.rowCount - 1) {
                                        tableIndex = shop_items_table.rowCount - 2
                                    }

                                    shop_items_table.currentRow = tableIndex
                                }
                            }
                        }

                        DelayButton {
                            text: qsTr("Remove All Items")
                            delay: 1200

                            onActivated: {
                                editor_tool.removeAllItems()
                                editor_tool.fillItemTable()
                            }
                        }
                    }
                }

                Column {
                    width: (parent.width - parent.spacing) / 2
                    height: parent.height
                    spacing: 5

                    Row {
                        id: item_header_row
                        width: parent.width
                        spacing: 5

                        Button {
                            text: qsTr("Add Selected Item")

                            onClicked: editor_tool.addListItem()
                        }

                        Button {
                            id: item_editor_button

                            text: qsTr("Open Item Editor")

                            onClicked: switchToItemEditor()
                        }
                    }

                    TabBar {
                        id: item_tab_bar
                        width: parent.width

                        onCurrentIndexChanged: {
                            console.log(currentIndex)

                            editor_tool.setItemListTabIndex(currentIndex)
                            editor_tool.loadItemList()
                            editor_tool.fillItemListTable()
                        }

                        Repeater {
                            id: item_tab_bar_repeater
                            model: 0

                            TabButton {
                                text: editor_tool.getItemListTabNames()[index]
                            }
                        }
                    }

                    Controls1_4.TableView {
                        id: item_list_table
                        width: parent.width
                        height: parent.height - parent.spacing * 2
                                - item_tab_bar.height - item_header_row.height

                        model: ListModel {
                            id: item_list_model
                        }

                        Component.onCompleted: {
                            editor_tool.loadItemListTabs()
                            item_tab_bar_repeater.model = editor_tool.getItemListTabNames()

                            editor_tool.loadItemList()
                            editor_tool.fillItemListTable()
                        }

                        onDoubleClicked: editor_tool.addListItem()

                        Controls1_4.TableViewColumn {
                            id: list_item_column
                            title: qsTr("Item")
                            role: "name"
                            movable: false
                            width: item_list_table.width / 6
                        }

                        Controls1_4.TableViewColumn {
                            id: list_price_column
                            title: qsTr("Price")
                            role: "price"
                            movable: false
                            width: item_list_table.width / 6
                        }

                        Controls1_4.TableViewColumn {
                            id: list_category_column
                            title: qsTr("Category")
                            role: "category"
                            movable: false
                            width: item_list_table.width / 6
                        }

                        Controls1_4.TableViewColumn {
                            id: list_description_column
                            title: qsTr("Description")
                            role: "description"
                            movable: false
                            width: item_list_table.width - list_item_column.width
                                   - list_price_column.width - list_category_column.width - 2
                        }
                    }
                }
            }
        }
    }
}
