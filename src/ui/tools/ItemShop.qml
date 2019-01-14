import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import "./shop"
import FontAwesome 2.0

Page {
    id: item_shop

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    function updateCategory(cat) {
        console.log("Changing category to " + cat)

        shop_tool.category = cat
    }

    function updateCategories() {
        shop_tool.loadCategories(project_combo_box.currentText)
        category_column.children = []
        shop_list_column.children = []

        var component = Qt.createComponent("./shop/CategoryButton.qml")

        for (var i = 0; i < shop_tool.categories.length; i++) {
            var button = component.createObject(category_column, {
                                                    "x": 0,
                                                    "y": 0,
                                                    "category": shop_tool.categories[i],
                                                    "parent_width": category_column.width
                                                })

            button.clicked.connect(updateCategory)
        }
    }

    function loadShop(shop) {
        shop_tool.load(project_combo_box.currentText, shop)
    }

    Connections {
        target: shop_tool

        onProjectsChanged: project_combo_box.model = shop_tool.projects

        onCategoryChanged: {
            console.log("Category Changed")

            shop_tool.loadShops(project_combo_box.currentText)
            shop_list_column.children = []

            var component = Qt.createComponent("./shop/ShopButton.qml")

            for (var i = 0; i < shop_tool.shops.length; i++) {
                var button = component.createObject(shop_list_column, {
                                                        "x": 0,
                                                        "y": 0,
                                                        "shop": shop_tool.shops[i]
                                                    })

                button.clicked.connect(loadShop)
            }
        }

        onShopNameChanged: shop_title_text.text = shop_tool.shopName
        onShopOwnerChanged: shop_owner_text.text = shop_tool.shopOwner
        onShopDescriptionChanged: shop_description_text.text = shop_tool.shopDescription

        onItemsChanged: {
            list_model.clear()

            for (var i = 0; i < shop_tool.item_names.length; i++) {
                list_model.append({
                                      "item": shop_tool.item_names[i],
                                      "price": shop_tool.item_prices[i],
                                      "description": shop_tool.item_descriptions[i]
                                  })
            }
        }
    }

    StackView {
        id: shop_stack_view
        anchors.fill: parent
        initialItem: shop_page

        Row {
            id: shop_page
            spacing: 5
            padding: 5

            Column {
                id: control_column
                width: platform.isAndroid ? parent.width / 5 : 175
                height: parent.height - parent.topPadding - parent.bottomPadding
                spacing: 5

                ComboBox {
                    id: project_combo_box
                    width: parent.width
                    height: platform.isAndroid ? width / 6 : color_scheme.toolbarHeight

                    model: shop_tool.projects

                    onCurrentTextChanged: updateCategories()
                }

                Button {
                    id: open_editor_button
                    height: platform.isAndroid ? width / 6 : color_scheme.toolbarHeight
                    width: parent.width

                    Text {
                        x: 10
                        text: FontAwesome.edit
                        font.family: FontAwesome.familySolid
                        font.pointSize: 15
                        anchors.verticalCenter: parent.verticalCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    text: qsTr("Open Editor")
                    visible: !platform.isAndroid

                    onClicked: {
                        if (shop_stack_view.currentItem != shop_editor_page) {
                            item_editor_page.visible = false
                            shop_page.visible = false
                            shop_editor_page.visible = true

                            shop_stack_view.pop(null)
                            shop_stack_view.push(shop_editor_page)
                        }
                    }
                }

                ScrollView {
                    id: type_scroll_view
                    width: parent.width
                    height: parent.height - parent.spacing * 2
                            - project_combo_box.height - open_editor_button.height

                    clip: true

                    Column {
                        id: category_column
                        width: control_column.width
                        spacing: 5
                    }
                }
            }

            Column {
                width: parent.width - parent.spacing * 2 - parent.leftPadding
                       - parent.rightPadding - control_column.width - shop_sidebar.width
                height: parent.height - parent.topPadding - parent.bottomPadding
                spacing: 5

                Row {
                    id: title_row
                    width: parent.width
                    height: color_scheme.toolbarHeight
                    spacing: 5

                    Text {
                        id: shop_title_text
                        text: qsTr("No Shop Selected")
                        font.pointSize: 20
                        font.bold: true
                        color: color_scheme.textColor
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Column {
                        width: parent.width - shop_title_text.width - parent.spacing
                        anchors.verticalCenter: parent.verticalCenter

                        Text {
                            id: shop_owner_text
                            text: qsTr("Owner: No Owner Available")
                            color: color_scheme.textColor
                            font.pointSize: 10
                        }

                        Text {
                            id: shop_description_text
                            text: qsTr("Description: No Description Available")
                            color: color_scheme.textColor
                            font.pointSize: 10
                        }
                    }
                }

                Rectangle {
                    id: table_header
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: color_scheme.toolbarHeight
                    color: color_scheme.listHeaderBackgroundColor

                    Row {
                        anchors.fill: parent
                        padding: 10
                        spacing: 20

                        Text {
                            text: qsTr("Item")
                            anchors.verticalCenter: parent.verticalCenter
                            width: parent.width / 5
                            horizontalAlignment: Text.AlignHCenter
                            font.pointSize: 12
                            color: color_scheme.listHeaderTextColor
                        }

                        Text {
                            text: qsTr("Price")
                            anchors.verticalCenter: parent.verticalCenter
                            width: parent.width / 5
                            horizontalAlignment: Text.AlignHCenter
                            font.pointSize: 12
                            color: color_scheme.listHeaderTextColor
                        }

                        Text {
                            text: qsTr("Description")
                            anchors.verticalCenter: parent.verticalCenter
                            width: parent.width * (3 / 5)
                            horizontalAlignment: Text.AlignHCenter
                            font.pointSize: 12
                            color: color_scheme.listHeaderTextColor
                        }
                    }
                }

                ListView {
                    id: list_view
                    anchors.left: parent.left
                    anchors.right: parent.right

                    height: parent.height - y

                    clip: true
                    spacing: 0

                    ScrollBar.vertical: ScrollBar {
                    }

                    model: ListModel {
                        id: list_model
                    }

                    delegate: Rectangle {
                        height: delegate_row.height
                        anchors.left: parent.left
                        anchors.right: parent.right
                        color: "transparent"

                        Row {
                            id: delegate_row
                            padding: 5
                            leftPadding: 10
                            rightPadding: 10
                            spacing: 20
                            anchors.left: parent.left
                            anchors.right: parent.right
                            height: color_scheme.toolbarHeight

                            Text {
                                text: item
                                color: color_scheme.textColor
                                width: (parent.width - parent.padding * 2 - parent.spacing * 2) / 5
                                clip: true
                                elide: Text.ElideRight
                                anchors.verticalCenter: parent.verticalCenter
                                font.pointSize: 12
                                font.bold: true
                            }

                            Text {
                                text: price
                                color: color_scheme.textColor
                                width: (parent.width - parent.padding * 2 - parent.spacing * 2) / 5
                                clip: true
                                elide: Text.ElideRight
                                anchors.verticalCenter: parent.verticalCenter
                                font.pointSize: 12
                                horizontalAlignment: Text.AlignRight
                            }

                            Item {
                                width: (parent.width - parent.padding * 2
                                        - parent.spacing * 2) * (3 / 5)
                                anchors.verticalCenter: parent.verticalCenter
                                height: item_desc_text.height

                                Text {
                                    id: item_desc_text
                                    text: description
                                    color: color_scheme.textColor

                                    clip: true
                                    elide: Text.ElideRight

                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    font.pointSize: 12
                                }

                                ToolTip {
                                    id: item_desc_tooltip
                                    text: description
                                }

                                MouseArea {
                                    hoverEnabled: true
                                    id: item_desc_mouse_area
                                    anchors.fill: parent

                                    onEntered: item_desc_tooltip.visible = true
                                    onExited: item_desc_tooltip.visible = false
                                }
                            }
                        }
                    }
                }
            }

            Column {
                id: shop_sidebar
                width: platform.isAndroid ? parent.width / 5 : 175
                height: parent.height

                ScrollView {
                    width: parent.width
                    height: parent.height
                    clip: true

                    Column {
                        id: shop_list_column
                        width: shop_sidebar.width
                        height: shop_sidebar.height

                        spacing: 5
                    }
                }
            }
        }
    }

    ShopEditor {
        id: shop_editor_page
        visible: false

        onBackToViewer: {
            if (shop_stack_view.currentItem != shop_page) {
                item_editor_page.visible = false
                shop_page.visible = true
                shop_editor_page.visible = false

                shop_stack_view.pop(null)
                shop_stack_view.push(shop_page)
            }
        }

        onSwitchToItemEditor: {
            if (shop_stack_view.currentItem != item_editor_page) {
                item_editor_page.visible = true
                shop_page.visible = false
                shop_editor_page.visible = false

                shop_stack_view.pop(null)
                shop_stack_view.push(item_editor_page)
            }
        }

        onProjectsChanged: {
            project_combo_box.model = shop_tool.projects
        }
    }

    ItemEditor {
        id: item_editor_page
        visible: false

        onBackToShopEditor: {
            if (shop_stack_view.currentItem != shop_editor_page) {
                item_editor_page.visible = false
                shop_page.visible = false
                shop_editor_page.visible = true

                shop_stack_view.pop(null)
                shop_stack_view.push(shop_editor_page)
            }
        }
    }
}
