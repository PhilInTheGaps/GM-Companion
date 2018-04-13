import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4 as Controls1_4

import "./shop"
import gm.companion.projectconverter 1.0
import gm.companion.shoptool 1.0

Page {
    id: item_shop

    ProjectConverter {
        id: project_converter

        Component.onCompleted: convert()
    }

    ShopTool {
        id: shop_tool

        onProjectsChanged: project_combo_box.model = projects

        function updateCategory(cat) {
            console.log("Changing category to " + cat)

            category = cat
        }

        function updateCategories() {
            loadCategories(project_combo_box.currentText)
            category_column.children = []
            shop_flow.children = []

            var component = Qt.createComponent("./shop/CategoryButton.qml")

            for (var i = 0; i < categories.length; i++) {
                var button = component.createObject(category_column, {
                                                        x: 0,
                                                        y: 0,
                                                        category: categories[i],
                                                        parent_width: category_column.width
                                                    })

                button.clicked.connect(updateCategory)
            }
        }

        onCategoryChanged: {
            console.log("Category Changed")

            loadShops(project_combo_box.currentText)
            shop_flow.children = []

            var component = Qt.createComponent("./shop/ShopButton.qml")

            for (var i = 0; i < shops.length; i++) {
                var button = component.createObject(shop_flow, {
                                                        x: 0,
                                                        y: 0,
                                                        shop: shops[i]
                                                    })

                button.clicked.connect(loadShop)
            }
        }

        function loadShop(shop) {
            load(project_combo_box.currentText, shop)
        }

        onShopNameChanged: shop_title_text.text = shopName
        onShopOwnerChanged: shop_owner_text.text = shopOwner
        onShopDescriptionChanged: shop_description_text.text = shopDescription

        onItemsChanged: {
            table_model.clear()

            for (var i = 0; i < item_names.length; i++) {
                table_model.append({
                                       item: item_names[i],
                                       price: item_prices[i],
                                       description: item_descriptions[i]
                                   })
            }
        }
    }

    SwipeView {
        id: shop_swipe_view
        anchors.fill: parent
        padding: 5
        spacing: 5
        interactive: false

        Row {
            spacing: 5

            Column {
                id: control_column
                width: 150
                height: parent.height - parent.topPadding - parent.bottomPadding
                spacing: 5

                Text {
                    id: projects_text
                    text: qsTr("Projects")
                }

                ComboBox {
                    id: project_combo_box
                    width: parent.width

                    model: shop_tool.projects

                    onCurrentTextChanged: shop_tool.updateCategories()
                }

                Button {
                    id: open_editor_button
                    width: parent.width
                    text: qsTr("Open Editor")

                    onClicked: shop_swipe_view.setCurrentIndex(1)
                }

                ScrollView {
                    id: type_scroll_view
                    width: parent.width
                    height: parent.height - parent.spacing * 3 - projects_text.height
                            - project_combo_box.height - open_editor_button.height

                    Column {
                        id: category_column
                        width: control_column.width
                    }
                }
            }

            Column {
                width: parent.width - parent.spacing - parent.leftPadding
                       - parent.rightPadding - control_column.width
                height: parent.height - parent.topPadding - parent.bottomPadding
                spacing: 5

                Flow {
                    id: shop_flow
                    width: parent.width

                    spacing: 5
                }

                Row {
                    id: title_row
                    width: parent.width
                    spacing: 5

                    Text {
                        id: shop_title_text
                        text: qsTr("No Shop Selected")
                        font.pointSize: 20
                    }

                    Column {
                        width: parent.width - shop_title_text.width - parent.spacing

                        Text {
                            id: shop_owner_text
                            text: qsTr("Owner: No Owner Available")
                        }

                        Text {
                            id: shop_description_text
                            text: qsTr("Description: No Description Available")
                        }
                    }
                }

                ListModel {
                    id: table_model
                }

                Controls1_4.TableView {
                    id: shop_table

                    width: parent.width
                    height: parent.height - parent.spacing * 2 - shop_flow.height - title_row.height

                    model: table_model

                    Controls1_4.TableViewColumn {
                        id: item_column
                        title: qsTr("Item")
                        role: "item"
                        movable: false
                    }

                    Controls1_4.TableViewColumn {
                        id: price_column
                        title: qsTr("Price")
                        role: "price"
                        movable: false
                    }

                    Controls1_4.TableViewColumn {
                        id: description_column
                        title: qsTr("Description")
                        role: "description"
                        movable: false
                        width: shop_table.width - item_column.width - price_column.width - 5
                    }
                }
            }
        }

        ShopEditor {
            id: shop_editor

            onBackToViewer: shop_swipe_view.setCurrentIndex(0)
            onSwitchToItemEditor: shop_swipe_view.setCurrentIndex(2)
        }

        ItemEditor {
            id: item_editor

            onBackToShopEditor: shop_swipe_view.setCurrentIndex(1)
        }
    }
}
