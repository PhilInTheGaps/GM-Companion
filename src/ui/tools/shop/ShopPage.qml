import QtQuick 2.9
import QtQuick.Controls 2.2

import FontAwesome 2.0
import "../../components"

Page {
    id: root

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    header: Rectangle {
        color: color_scheme.menuColor
        height: color_scheme.toolbarHeight

        Row {
            id: top_left_row
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 5
            width: category_scroll_view.width
            spacing: 5

            // Project ComboBox
            ToolBarComboBox {
                id: project_combo_box
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: parent.width - editor_button.width - parent.spacing
                model: shop_tool.projects
                onCurrentTextChanged: shop_tool.setCurrentProject(currentIndex)
            }

            // Open Editor Button
            ToolBarIconButton {
                id: editor_button
                fa_icon: FontAwesome.edit
                anchors.margins: 2

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

            // Shop Name
            Label {
                id: shop_name_label
                text: shop_tool.shopName
                font.pixelSize: height - 10
                font.bold: true
                color: color_scheme.toolbarTextColor
                anchors.top: parent.top
                anchors.bottom: parent.bottom
            }

            // Owner and Description
            Column {
                width: parent.width - x
                anchors.verticalCenter: parent.verticalCenter
                leftPadding: 10

                Label {
                    id: shop_owner_text
                    text: shop_tool.shopOwner
                    color: color_scheme.toolbarTextColor
                    font.pointSize: 10
                }

                Label {
                    id: shop_description_text
                    text: shop_tool.shopDescription
                    color: color_scheme.toolbarTextColor
                    font.pointSize: 10
                }
            }
        }
    }

    contentItem: Item {
        id: main_item

        // Left
        ScrollView {
            id: category_scroll_view
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 5
            width: platform.isAndroid ? parent.width / 5 : 175

            clip: true
            contentHeight: category_column.implicitHeight

            Column {
                id: category_column
                anchors.left: parent.left
                anchors.right: parent.right
                spacing: 5

                Repeater {
                    model: shop_tool.categories

                    CategoryButton {
                        category: modelData
                        onClicked: shop_tool.setCurrentCategory(index)
                    }
                }
            }
        }

        // Mid
        Item {
            id: mid_item
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: category_scroll_view.right
            anchors.right: shop_scroll_view.left
            anchors.margins: 5

            Rectangle {
                id: table_header
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                height: color_scheme.toolbarHeight - 10
                color: color_scheme.listHeaderBackgroundColor

                Row {
                    anchors.fill: parent
                    padding: 10
                    spacing: 20

                    Text {
                        text: qsTr("Item")
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.width / 5
                        horizontalAlignment: Text.AlignLeft
                        font.pointSize: 12
                        color: color_scheme.listHeaderTextColor
                    }

                    Text {
                        text: qsTr("Price")
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.width / 5
                        horizontalAlignment: Text.AlignRight
                        font.pointSize: 12
                        color: color_scheme.listHeaderTextColor
                    }

                    Text {
                        text: qsTr("Description")
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.width * (3 / 5)
                        horizontalAlignment: Text.AlignLeft
                        font.pointSize: 12
                        color: color_scheme.listHeaderTextColor
                    }
                }
            }

            ListView {
                id: list_view
                anchors.top: table_header.bottom
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.topMargin: 5

                clip: true
                spacing: 0

                ScrollBar.vertical: ScrollBar {
                }

                model: shopItemModel

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
                        height: description_label.height > color_scheme.toolbarHeight
                                - 10 ? description_label.height : color_scheme.toolbarHeight - 10

                        Text {
                            text: modelData.name
                            color: color_scheme.textColor
                            width: table_header.width / 5
                            clip: true
                            elide: Text.ElideRight
                            anchors.top: parent.top
                            topPadding: 5
                            font.pointSize: 12
                            font.bold: true
                        }

                        Text {
                            text: modelData.price
                            color: color_scheme.textColor
                            width: table_header.width / 5
                            clip: true
                            elide: Text.ElideRight
                            anchors.top: parent.top
                            topPadding: 5
                            font.pointSize: 12
                            horizontalAlignment: Text.AlignRight
                        }

                        Label {
                            id: description_label
                            text: modelData.description
                            topPadding: 5
                            font.pointSize: 12
                            width: parent.width - x - parent.rightPadding
                            anchors.top: parent.top
                            wrapMode: Text.WordWrap
                            color: color_scheme.textColor
                        }
                    }
                }
            }
        }

        // Right
        ScrollView {
            id: shop_scroll_view
            width: platform.isAndroid ? parent.width / 5 : 175
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 5
            clip: true
            contentHeight: shop_list_column.implicitHeight

            Column {
                id: shop_list_column
                anchors.left: parent.left
                anchors.right: parent.right
                spacing: 5

                Repeater {
                    model: shop_tool.shops

                    ShopButton {
                        shop: modelData
                        onClicked: shop_tool.setCurrentShop(index)
                    }
                }
            }
        }
    }
}
