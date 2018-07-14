import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2

import gm.companion.itemeditor 1.0
import "../../fontawesome"

Page {
    id: editor

    signal backToShopEditor

    Component.onCompleted: editor_tool.updateCategories()

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

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

    header: header_bar

    Rectangle {
        id: header_bar
        color: color_scheme.toolbarColor
        height: color_scheme.toolbarHeight

        Button {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            hoverEnabled: true

            background: Rectangle {
                color: "transparent"
            }

            Icon {
                icon: icons.far_arrow_alt_circle_left
                pointSize: 25
                anchors.centerIn: parent
                color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
            }

            width: height
            onClicked: backToShopEditor()
        }
    }

    Row {
        width: parent.width
        height: parent.height
        spacing: 5
        padding: 5

        Column {
            id: item_properties_column
            width: (parent.width - parent.spacing * 2 - parent.padding * 2 - mid_spacer.width) / 3
            height: parent.height - parent.padding * 2
            spacing: 5

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

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                height: 1
                color: color_scheme.dividerColor
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

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                height: 1
                color: color_scheme.dividerColor
            }

            TextArea {
                width: parent.width
                padding: 0
                readOnly: true
                wrapMode: "WordWrap"
                clip: true

                color: color_scheme.textColor
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

        Rectangle {
            id: mid_spacer
            height: parent.height - parent.padding * 2
            width: 1
            color: color_scheme.dividerColor
        }

        Column {
            id: table_column
            width: parent.width - parent.spacing * 2 - parent.padding * 2
                   - item_properties_column.width - mid_spacer.width
            height: parent.height - parent.padding * 2
            spacing: 5

            Rectangle {
                id: item_header
                anchors.left: parent.left
                anchors.right: parent.right
                height: color_scheme.toolbarHeight
                color: color_scheme.listHeaderBackgroundColor

                Row {
                    anchors.fill: parent
                    padding: 5
                    leftPadding: 10
                    rightPadding: 10
                    spacing: 10

                    Text {
                        text: qsTr("Item")
                        color: color_scheme.listHeaderTextColor
                        font.pointSize: 12
                        font.bold: true
                        width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 6
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Text {
                        text: qsTr("Price")
                        color: color_scheme.listHeaderTextColor
                        font.pointSize: 12
                        font.bold: true
                        width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 6
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Text {
                        text: qsTr("Category")
                        color: color_scheme.listHeaderTextColor
                        font.pointSize: 12
                        font.bold: true
                        width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 6
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Text {
                        text: qsTr("Description")
                        color: color_scheme.listHeaderTextColor
                        font.pointSize: 12
                        font.bold: true
                        width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 2
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }

            ListView {
                id: item_table_view
                anchors.left: parent.left
                anchors.right: parent.right
                height: parent.height

                clip: true
                spacing: 0

                ScrollBar.vertical: ScrollBar {
                }

                model: ListModel {
                    id: table_model
                }

                Component.onCompleted: editor_tool.updateItems()

                delegate: Rectangle {
                    height: delegate_row.height
                    anchors.left: parent.left
                    anchors.right: parent.right
                    color: "transparent"

                    MouseArea {
                        id: mouse_area
                        anchors.fill: parent
                        hoverEnabled: true

                        z: 2
                        onClicked: mouse.accepted = false
                        onPressed: mouse.accepted = false
                        onReleased: mouse.accepted = false
                        onDoubleClicked: mouse.accepted = false
                        onPositionChanged: mouse.accepted = false
                        onPressAndHold: mouse.accepted = false
                    }

                    Row {
                        id: delegate_row
                        padding: 5
                        leftPadding: 10
                        rightPadding: 10
                        spacing: 10
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: 30

                        Text {
                            text: name
                            color: color_scheme.textColor
                            width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 6
                            clip: true
                            elide: Text.ElideRight
                            anchors.verticalCenter: parent.verticalCenter
                            font.pointSize: 12
                        }

                        Text {
                            text: price
                            color: color_scheme.textColor
                            width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 6
                            clip: true
                            elide: Text.ElideRight
                            anchors.verticalCenter: parent.verticalCenter
                            font.pointSize: 12
                        }

                        Text {
                            text: category
                            color: color_scheme.textColor
                            width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 6
                            clip: true
                            elide: Text.ElideRight
                            anchors.verticalCenter: parent.verticalCenter
                            font.pointSize: 12
                        }

                        Text {
                            text: description
                            color: color_scheme.textColor
                            width: (parent.width - x - delete_button.width
                                    - parent.spacing - parent.rightPadding)
                            clip: true
                            elide: Text.ElideRight
                            anchors.verticalCenter: parent.verticalCenter
                            font.pointSize: 12
                        }

                        Button {
                            id: delete_button
                            height: parent.height - parent.topPadding * 2
                            width: height
                            hoverEnabled: true
                            visible: mouse_area.containsMouse

                            background: Rectangle {
                                color: "transparent"
                            }

                            Icon {
                                icon: icons.fas_times
                                pointSize: 20
                                anchors.centerIn: parent
                                color: parent.pressed ? "grey" : (parent.hovered ? "lightgrey" : color_scheme.primaryButtonColor)
                            }

                            onClicked: editor_tool.deleteItem(index)
                        }
                    }
                }
            }
        }
    }
}
