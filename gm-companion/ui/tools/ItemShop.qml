import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4

import "./shop"

Page {
    id: item_shop

    SwipeView {
        id: shop_swipe_view
        anchors.fill: parent
        padding: 5
        spacing: 5

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

                Row {
                    id: load_project_row
                    width: parent.width
                    spacing: 5

                    ComboBox {
                        id: project_combo_box
                    }

                    Button {
                        id: load_project_button
                        text: qsTr("Load")
                    }
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
                            - load_project_row.height - open_editor_button.height
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

                TableView {
                    id: shop_table

                    width: parent.width
                    height: parent.height - parent.spacing * 2 - shop_flow.height - title_row.height

                    TableViewColumn {
                        id: item_column
                        title: qsTr("Item")
                        role: "item"
                        movable: false
                    }

                    TableViewColumn {
                        id: price_column
                        title: qsTr("Price")
                        role: "price"
                        movable: false
                    }

                    TableViewColumn {
                        id: description_column
                        title: qsTr("Description")
                        role: "description"
                        movable: false
                        width: parent.width - item_column.width - price_column.width
                    }
                }
            }
        }

        ShopEditor {
            id: shop_editor

            onBackToViewer: shop_swipe_view.setCurrentIndex(0)
        }

        ItemEditor {
            id: item_editor
        }
    }
}
