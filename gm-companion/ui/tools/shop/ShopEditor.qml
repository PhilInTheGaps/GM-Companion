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

            shop_column.children = []

            var component = Qt.createComponent("./ShopButton.qml")

            console.log(getShopList())

            for (var i = 0; i < getShopList().length; i++) {

                console.log(getShopList()[i])

                var button = component.createObject(shop_column, {
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

    Column {
        width: parent.width
        height: parent.height
        spacing: 5

        Dialog {
            id: new_project_dialog
            title: qsTr("Create New Project")

            contentItem: Column {
                TextField {
                    id: project_textfield
                    width: parent.width
                    placeholderText: qsTr("Project Name")
                    selectByMouse: true
                }
            }

            standardButtons: Dialog.Ok | Dialog.Cancel

            onAccepted: {
                editor_tool.createProject(project_textfield.text)

                project_combo_box.model = shop_tool.projects
            }
        }

        Dialog {
            id: new_category_dialog
            title: qsTr("Create New Category")

            contentItem: Column {

                TextField {
                    id: category_textfield
                    width: parent.width
                    placeholderText: qsTr("Category Name")
                    selectByMouse: true
                }
            }

            standardButtons: Dialog.Ok | Dialog.Cancel

            onAccepted: {
                editor_tool.createCategory(category_textfield.text)

                category_combo_box.model = editor_tool.getCategories()

                category_combo_box.currentIndex = editor_tool.getCategories(
                            ).length - 1
            }
        }

        Dialog {
            id: new_shop_dialog
            title: qsTr("Create New Shop")

            contentItem: Column {

                TextField {
                    id: shop_name_textfield
                    width: parent.width
                    placeholderText: qsTr("Shop Name")
                    selectByMouse: true
                }
            }

            standardButtons: Dialog.Ok | Dialog.Cancel

            onAccepted: {
                editor_tool.createShop(shop_name_textfield.text)

                editor_tool.updateShopFlow()
            }
        }

        Row {
            id: tool_bar
            width: parent.height
            spacing: 5

            Button {

                ToolTip.text: qsTr("Back")
                ToolTip.visible: hovered
                hoverEnabled: true

                Image {
                    source: "/icons/media/playBackwards.png"
                    width: parent.height * 0.9
                    height: width

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter

                    sourceSize.width: width
                    sourceSize.height: height
                }

                width: height
                onClicked: backToViewer()
            }

            Button {
                text: qsTr("Create New Project")

                onClicked: {
                    new_project_dialog.open()
                }
            }

            Button {
                text: qsTr("Create New Category")

                onClicked: {
                    new_category_dialog.open()
                }
            }

            Button {
                text: qsTr("Create New Shop")

                onClicked: {
                    new_shop_dialog.open()
                }
            }
        }

        Row {
            width: parent.width
            height: parent.height - parent.spacing - tool_bar.height
            spacing: 5

            Column {
                id: shop_properties_column
                width: parent.width / 8
                height: parent.height
                spacing: 5

                Text {
                    id: project_text
                    text: qsTr("Project")
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
                    id: category_text
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

                Text {
                    id: shops_text
                    text: qsTr("Shops")
                }

                ScrollView {
                    width: parent.width
                    height: parent.height - parent.spacing * 5 - category_combo_box.height
                            - category_text.height - shops_text.height
                            - project_combo_box.height - project_text.height

                    clip: true

                    Column {
                        id: shop_column
                        width: parent.parent.width
                        spacing: 5
                    }
                }
            }

            Column {
                width: parent.width - parent.spacing - shop_properties_column.width
                height: parent.height
                spacing: 5

                Row {
                    width: parent.width
                    height: parent.height
                    spacing: 5

                    Column {
                        width: (parent.width - parent.spacing) / 2
                        height: parent.height
                        spacing: 5

                        Text {
                            id: shop_name_text
                            text: qsTr("No Shop Selected")
                            font.pointSize: 20

                            width: parent.width
                            clip: true
                            elide: Text.ElideRight
                        }

                        Row {
                            id: shop_properties_row
                            width: parent.width
                            spacing: 5

                            TextField {
                                id: shop_owner_textfield
                                width: parent.width / 4
                                placeholderText: qsTr("Shop Owner")
                                selectByMouse: true

                                onTextEdited: editor_tool.setShopOwner(text)
                            }

                            TextField {
                                id: shop_description_textfield
                                width: parent.width - parent.spacing - shop_owner_textfield.width
                                placeholderText: qsTr("Shop Description")
                                selectByMouse: true

                                onTextEdited: editor_tool.setShopDescription(
                                                  text)
                            }
                        }

                        Row {
                            id: shop_action_row
                            width: parent.width
                            spacing: 5

                            Button {
                                width: (parent.width - parent.spacing * 3) / 4
                                text: qsTr("Save Shop")

                                onClicked: editor_tool.saveShop()
                            }

                            DelayButton {
                                width: (parent.width - parent.spacing * 3) / 4
                                text: qsTr("Delete Shop")
                                delay: 1200

                                onActivated: {
                                    editor_tool.deleteShop()

                                    editor_tool.updateShopFlow()
                                }
                            }

                            Button {
                                width: (parent.width - parent.spacing * 3) / 4
                                text: qsTr("Remove Selected Item")

                                ToolTip.text: qsTr("Remove Selected Item")
                                ToolTip.visible: hovered
                                hoverEnabled: true

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
                                width: (parent.width - parent.spacing * 3) / 4
                                text: qsTr("Remove All Items")
                                delay: 1200

                                ToolTip.text: qsTr("Remove All Items")
                                ToolTip.visible: hovered
                                hoverEnabled: true

                                onActivated: {
                                    editor_tool.removeAllItems()
                                    editor_tool.fillItemTable()
                                }
                            }
                        }

                        Controls1_4.TableView {
                            id: shop_items_table
                            width: parent.width
                            height: parent.height - parent.spacing * 3 - shop_properties_row.height
                                    - shop_name_text.height - shop_action_row.height

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
                    }

                    Column {
                        width: (parent.width - parent.spacing) / 2
                        height: parent.height
                        spacing: 5

                        TabBar {
                            id: item_tab_bar
                            width: parent.width

                            spacing: 5

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
                                    text: editor_tool.getItemListTabNames(
                                              )[index]
                                }
                            }
                        }

                        Row {
                            id: item_header_row
                            width: parent.width
                            spacing: 5

                            Button {
                                width: (parent.width - parent.spacing) / 2
                                text: qsTr("Add Selected Item")

                                onClicked: editor_tool.addListItem()
                            }

                            Button {
                                id: item_editor_button
                                width: (parent.width - parent.spacing) / 2
                                text: qsTr("Open Item Editor")

                                onClicked: switchToItemEditor()
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
}
