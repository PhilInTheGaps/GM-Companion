import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4 as Controls1_4

import gm.companion.shoptool 1.0

Page {
    id: editor

    signal backToViewer
    signal switchToItemEditor

    ShopTool {
        id: shop_tool
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

            ComboBox {
                id: project_combo_box
                width: parent.width
                model: shop_tool.projects
            }

            TextField {
                width: parent.width
                placeholderText: qsTr("Project Name")
                selectByMouse: true
            }

            Button {
                width: parent.width
                text: qsTr("Create New Project")
            }

            Text {
                text: qsTr("New Category")
            }

            TextField {
                width: parent.width
                placeholderText: qsTr("Category Name")
                selectByMouse: true
            }

            Button {
                width: parent.width
                text: qsTr("Create New Category")
            }

            Text {
                text: qsTr("New Shop")
            }

            TextField {
                width: parent.width
                placeholderText: qsTr("Shop Name")
                selectByMouse: true
            }

            ComboBox {
                width: parent.width
                Component.onCompleted: {
                    shop_tool.loadCategories(project_combo_box.currentText)
                    model = shop_tool.categories
                }
            }

            Button {
                width: parent.width
                text: qsTr("Create New Shop")
            }

            ScrollView {
                width: parent.width

                Column {
                    width: parent.width
                }
            }
        }

        Column {
            width: parent.width - parent.spacing - shop_properties_column.width
            height: parent.height
            spacing: 5

            Flow {
                id: shop_flow
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
                        }

                        TextField {
                            id: shop_owner_textfield
                            width: parent.width - shop_name_text.width - parent.spacing
                            placeholderText: qsTr("Shop Owner")
                            selectByMouse: true
                        }
                    }

                    TextField {
                        id: shop_description_textfield
                        width: parent.width
                        placeholderText: qsTr("Shop Description")
                        selectByMouse: true
                    }

                    Controls1_4.TableView {
                        width: parent.width
                        height: parent.height - parent.spacing * 2 - shop_properties_row.height
                                - shop_description_textfield.height
                    }
                }

                Column {
                    width: (parent.width - parent.spacing) / 2
                    height: parent.height
                    spacing: 5

                    Row {
                        id: item_header_row
                        width: parent.width

                        Button {
                            id: item_editor_button

                            text: qsTr("Open Item Editor")

                            onClicked: switchToItemEditor()
                        }
                    }

                    TabBar {
                        id: item_tab_bar
                        width: parent.width

                        TabButton {
                            text: "Test Items"
                        }

                        TabButton {
                            text: "Other Items"
                        }
                    }
                    SwipeView {
                        id: item_swipe_view
                        width: parent.width
                        height: parent.height - item_header_row.height
                                - item_tab_bar.height - parent.spacing * 2

                        currentIndex: item_tab_bar.currentIndex
                    }
                }
            }
        }
    }
}
