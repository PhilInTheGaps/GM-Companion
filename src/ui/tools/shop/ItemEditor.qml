import QtQuick 2.9
import QtQuick.Controls 2.2

import FontAwesome 2.0
import "../../components"

Page {
    id: editor

    signal backToShopEditor

    Connections {
        target: item_editor

        onShowInfoBar: {
            info_text.text = message
            info_bar.visible = true
            info_bar_timer.start()
        }
    }

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    Connections {
        target: item_editor

        onCategoriesChanged: category_combobox.model = item_editor.categories
        onItemsChanged: loadItemTable()
    }

    header: Rectangle {
        id: header_bar
        color: color_scheme.toolbarColor
        height: color_scheme.toolbarHeight

        Row {
            anchors.fill: parent

            // Back
            ToolBarIconButton {
                fa_icon: FontAwesome.arrowAltCircleLeft
                onClicked: backToShopEditor()
            }

            // Save
            ToolBarIconButton {
                fa_icon: FontAwesome.fileDownload
                onClicked: item_editor.save()

                Text {
                    visible: !item_editor.isSaved
                    text: FontAwesome.asterisk
                    color: "darkred"
                    font.family: FontAwesome.familySolid
                    anchors.right: parent.right
                    y: parent.height - height * 1.5
                }
            }
        }
    }

    Column {
        id: left_column
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 5
        width: 175

        spacing: 5

        TextField {
            id: category_name_field
            anchors.left: parent.left
            anchors.right: parent.right
            height: color_scheme.toolbarHeight
            placeholderText: qsTr("Category Name")
            selectByMouse: true
        }

        Button {
            id: add_category_button
            anchors.left: parent.left
            anchors.right: parent.right
            height: color_scheme.toolbarHeight
            text: qsTr("Add Category")

            onClicked: {
                if (category_name_field.text != "") {
                    item_editor.addCategory(category_name_field.text)
                }
            }
        }

        Rectangle {
            width: 5
            height: 5
            color: "transparent"
        }

        TextField {
            id: item_name_field
            anchors.left: parent.left
            anchors.right: parent.right
            height: color_scheme.toolbarHeight
            placeholderText: qsTr("Item Name")
            selectByMouse: true
        }

        TextField {
            id: item_price_field
            anchors.left: parent.left
            anchors.right: parent.right
            height: color_scheme.toolbarHeight
            placeholderText: qsTr("Price")
            selectByMouse: true
        }

        ComboBox {
            id: category_combobox
            anchors.left: parent.left
            anchors.right: parent.right
            height: color_scheme.toolbarHeight
            model: item_editor.categories
        }

        ScrollView {
            anchors.left: parent.left
            anchors.right: parent.right
            height: color_scheme.toolbarHeight * 4
            clip: true
            contentWidth: -1

            TextArea {
                id: item_description_field
                width: parent.width

                placeholderText: qsTr("Item Description")
                color: "black"
                selectByMouse: true
                wrapMode: Text.Wrap

                background: Rectangle {
                    color: "white"
                }
            }
        }

        Button {
            anchors.left: parent.left
            anchors.right: parent.right
            height: color_scheme.toolbarHeight
            text: qsTr("Add Item")

            onClicked: item_editor.addItem(item_name_field.text,
                                           item_price_field.text,
                                           category_combobox.currentText,
                                           item_description_field.text)
        }
    }

    Item {
        id: right_item

        anchors.left: left_column.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 5

        Rectangle {
            id: item_header
            anchors.top: parent.top
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
                    text: qsTr("Name")
                    color: color_scheme.listHeaderTextColor
                    font.pointSize: 12
                    font.bold: true
                    width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 5
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    text: qsTr("Price")
                    color: color_scheme.listHeaderTextColor
                    font.pointSize: 12
                    font.bold: true
                    width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 6
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: Text.AlignRight
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
            anchors.top: item_header.bottom
            anchors.bottom: parent.bottom
            anchors.topMargin: 5

            clip: true
            spacing: 0

            ScrollBar.vertical: ScrollBar {
            }

            model: itemEditorItemModel

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
                        text: modelData.name
                        color: color_scheme.textColor
                        width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 5
                        clip: true
                        elide: Text.ElideRight
                        anchors.verticalCenter: parent.verticalCenter
                        font.pointSize: 12
                    }

                    Text {
                        text: modelData.price
                        color: color_scheme.textColor
                        width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 6
                        clip: true
                        elide: Text.ElideRight
                        horizontalAlignment: Text.AlignRight
                        anchors.verticalCenter: parent.verticalCenter
                        font.pointSize: 12
                    }

                    Text {
                        text: modelData.category
                        color: color_scheme.textColor
                        width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 6
                        clip: true
                        elide: Text.ElideRight
                        anchors.verticalCenter: parent.verticalCenter
                        font.pointSize: 12
                    }

                    Text {
                        text: modelData.description
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
                            color: parent.pressed ? "grey" : (parent.hovered ? "lightgrey" : color_scheme.textColor)
                        }

                        onClicked: item_editor.deleteItem(index)
                    }
                }
            }
        }
    }

    Rectangle {
        id: info_bar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: color_scheme.toolbarHeight
        color: color_scheme.menuColor
        visible: false

        Timer {
            id: info_bar_timer
            interval: 2000

            onTriggered: {
                info_bar.visible = false
                stop()
            }
        }

        Text {
            id: info_text
            anchors.fill: parent
            anchors.margins: 10
            font.pixelSize: height
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: color_scheme.toolbarTextColor
        }
    }
}
