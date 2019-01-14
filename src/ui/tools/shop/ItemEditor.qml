import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2

import FontAwesome 2.0

Page {
    id: editor

    signal backToShopEditor

    Component.onCompleted: {
        item_editor.updateCategories()
        item_editor.updateItems()
    }

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    Connections {
        target: item_editor

        onCategoriesChanged: category_combobox.model = item_editor.categories
        onItemsChanged: loadItemTable()
    }

    function loadItemTable() {
        table_model.clear()

        for (var i = 0; i < item_editor.getItemNames().length; i++) {
            table_model.append({
                                   "name": item_editor.getItemNames()[i],
                                   "price": item_editor.getItemPrices()[i],
                                   "category": item_editor.getItemCategories(
                                                   )[i],
                                   "description": item_editor.getItemDescriptions(
                                                      )[i]
                               })
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

            Text {
                text: FontAwesome.arrowAltCircleLeft
                font.family: FontAwesome.familySolid
                font.pointSize: 25
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
                    selectByMouse: true
                }

                TextField {
                    id: item_price_field
                    width: (parent.width - parent.spacing * 2) / 3
                    placeholderText: qsTr("Price")
                    selectByMouse: true
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
                selectByMouse: true
            }

            Button {
                text: qsTr("Add Item")

                onClicked: {
                    if (item_name_field.text != ""
                            && category_combobox.currentText != "") {
                        item_editor.addItem(item_name_field.text,
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
                            item_editor.addCategory(category_name_field.text)
                        }
                    }
                }

                TextField {
                    id: category_name_field
                    width: parent.width - parent.spacing - add_category_button.width
                    placeholderText: qsTr("Category Name")
                    selectByMouse: true
                }
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
                height: parent.height - y - parent.bottomPadding

                clip: true
                spacing: 0

                ScrollBar.vertical: ScrollBar {
                }

                model: ListModel {
                    id: table_model
                }

                Component.onCompleted: item_editor.updateItems()

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

                            Text {
                                text: FontAwesome.times
                                font.family: FontAwesome.familySolid
                                font.pointSize: 20
                                anchors.centerIn: parent
                                color: parent.pressed ? "grey" : (parent.hovered ? "lightgrey" : color_scheme.primaryButtonColor)
                            }

                            onClicked: item_editor.deleteItem(index)
                        }
                    }
                }
            }
        }
    }
}
