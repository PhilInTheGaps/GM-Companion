import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2

import FontAwesome 2.0
import "../../components"

Page {
    id: root

    signal backToViewer
    signal switchToItemEditor

    Connections {
        target: shop_editor

        onShowInfoBar: {
            info_text.text = message
            info_bar.visible = true
            info_bar_timer.start()
        }
    }

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    EditorNewThingDialog {
        id: new_thing_dialog
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
    }

    header: Rectangle {
        id: tool_bar
        color: color_scheme.toolbarColor
        height: color_scheme.toolbarHeight

        Row {
            id: header_row_left
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            spacing: 5

            // Back
            ToolBarIconButton {
                fa_icon: FontAwesome.arrowAltCircleLeft
                onClicked: backToViewer()
            }

            // New thing
            ToolBarIconButton {
                fa_icon: FontAwesome.fileMedical
                onClicked: new_thing_dialog.open()
            }

            // Save
            ToolBarIconButton {
                fa_icon: FontAwesome.fileDownload
                onClicked: shop_editor.save()

                Text {
                    visible: !shop_editor.isSaved
                    text: FontAwesome.asterisk
                    color: "darkred"
                    font.family: FontAwesome.familySolid
                    anchors.right: parent.right
                    y: parent.height - height * 1.5
                }
            }

            // Projects
            ToolBarComboBox {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 5
                model: shop_editor.projectNames

                onCurrentIndexChanged: shop_editor.setCurrentProject(
                                           currentIndex)
            }

            // Categories
            ToolBarComboBox {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 5
                model: shop_editor.categoryNames

                onCurrentIndexChanged: shop_editor.setCurrentCategory(
                                           currentIndex)
            }
        }

        Row {
            id: header_row_right
            anchors.left: header_row_left.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.rightMargin: 5

            layoutDirection: Qt.RightToLeft

            // Item Editor
            ToolBarIconButton {
                fa_icon: FontAwesome.edit
                onClicked: switchToItemEditor()
            }

            // Item Groups
            ToolBarComboBox {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 5

                model: shop_editor.itemGroups
                onCurrentIndexChanged: shop_editor.setCurrentItemGroup(
                                           currentIndex)
            }
        }
    }

    // Left
    ScrollView {
        id: left_scroll_view
        width: 175
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 5

        clip: true

        Column {
            id: shop_column
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 5

            Repeater {
                model: shop_editor.shopNames

                ShopButton {
                    shop: modelData
                    width: left_scroll_view.width

                    onClicked: shop_editor.setCurrentShop(index)
                }
            }
        }
    }

    // Mid
    Column {
        id: mid_column
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: left_scroll_view.right
        anchors.right: right_column.left
        anchors.margins: 5
        spacing: 5

        // Shop Info
        Item {
            id: shop_info_item
            anchors.left: parent.left
            anchors.right: parent.right
            height: color_scheme.toolbarHeight

            // Move up and down
            Item {
                id: shop_up_down
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: height / 2

                // Up
                Button {
                    id: shop_up
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: parent.height / 2

                    background: Rectangle {
                        color: color_scheme.menuColor
                    }

                    Text {
                        text: shop_name_field.edit_mode ? FontAwesome.checkCircle : FontAwesome.chevronUp
                        font.family: FontAwesome.familySolid
                        color: shop_name_field.edit_mode ? "limegreen" : color_scheme.toolbarTextColor
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    onClicked: {
                        if (shop_name_field.edit_mode) {
                            shop_name_field.edit_mode = false
                            shop_editor.name = shop_name_field.text
                        } else {
                            shop_editor.moveShop(-1)
                        }
                    }
                }

                // Down
                Button {
                    id: shop_down_down
                    anchors.top: shop_up.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom

                    background: Rectangle {
                        color: color_scheme.menuColor
                    }

                    Text {
                        text: shop_name_field.edit_mode ? FontAwesome.timesCircle : FontAwesome.chevronDown
                        font.family: FontAwesome.familySolid
                        color: shop_name_field.edit_mode ? "red" : color_scheme.toolbarTextColor
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    onClicked: {
                        if (shop_name_field.edit_mode) {
                            shop_name_field.edit_mode = false
                            shop_name_field.text = shop_editor.name
                        } else {
                            shop_editor.moveShop(1)
                        }
                    }
                }
            }

            // Name of shop
            TextField {
                id: shop_name_field
                text: shop_editor.name
                anchors.left: shop_up_down.right
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                selectByMouse: true

                property bool edit_mode: false
                activeFocusOnPress: edit_mode
                focus: edit_mode

                Button {
                    anchors.top: parent.top
                    anchors.right: shop_delete_button.left
                    anchors.bottom: parent.bottom
                    width: height
                    visible: !shop_name_field.edit_mode

                    background: Rectangle {
                        color: "transparent"
                    }

                    Text {
                        text: FontAwesome.pen
                        font.family: FontAwesome.familySolid
                        anchors.fill: parent
                        anchors.margins: 10
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    onClicked: shop_name_field.edit_mode = true
                }

                // Delete Shop
                Button {
                    id: shop_delete_button
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    width: height

                    background: Rectangle {
                        color: color_scheme.menuColor
                    }

                    Text {
                        text: FontAwesome.trashAlt
                        font.family: FontAwesome.familySolid
                        font.pixelSize: height
                        anchors.fill: parent
                        anchors.margins: 10
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        color: "red"
                    }

                    onClicked: shop_delete_overlay.visible = true

                    // Confirm deletion
                    Rectangle {
                        id: shop_delete_overlay
                        visible: false
                        anchors.fill: parent
                        color: color_scheme.menuColor

                        Text {
                            text: FontAwesome.trashAlt
                            font.family: FontAwesome.familySolid
                            color: color_scheme.toolbarTextColor
                            font.pixelSize: parent.height / 3
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            anchors.left: parent.left
                            width: height / 2
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }

                        // Yes
                        Button {
                            anchors.top: parent.top
                            anchors.right: parent.right
                            width: parent.width / 2
                            height: parent.height / 2

                            background: Rectangle {
                                color: "transparent"
                            }

                            Text {
                                text: FontAwesome.checkCircle
                                font.family: FontAwesome.familySolid
                                color: "limegreen"
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }

                            onClicked: {
                                shop_editor.deleteShop()
                                shop_delete_overlay.visible = false
                            }
                        }

                        // No
                        Button {
                            anchors.bottom: parent.bottom
                            anchors.right: parent.right
                            width: parent.width / 2
                            height: parent.height / 2

                            background: Rectangle {
                                color: "transparent"
                            }

                            Text {
                                text: FontAwesome.timesCircle
                                font.family: FontAwesome.familySolid
                                color: "red"
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }

                            onClicked: {
                                shop_delete_overlay.visible = false
                            }
                        }
                    }
                }
            }
        }

        // Further info
        Row {
            id: shop_properties_row
            width: parent.width
            spacing: 5

            // Owner
            TextField {
                id: shop_owner_textfield
                width: parent.width / 4
                placeholderText: qsTr("Shop Owner")
                selectByMouse: true
                text: shop_editor.owner

                onTextEdited: shop_editor.setOwner(text)
            }

            // Description
            TextField {
                id: shop_description_textfield
                width: parent.width - parent.spacing - shop_owner_textfield.width
                placeholderText: qsTr("Shop Description")
                selectByMouse: true
                text: shop_editor.description

                onTextEdited: shop_editor.setDescription(text)
            }
        }

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
                    width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 4
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    text: qsTr("Price")
                    color: color_scheme.listHeaderTextColor
                    font.pointSize: 12
                    font.bold: true
                    width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 5
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: Text.AlignRight
                }

                Text {
                    text: qsTr("Category")
                    color: color_scheme.listHeaderTextColor
                    font.pointSize: 12
                    font.bold: true
                    width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 5
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    text: qsTr("Description")
                    color: color_scheme.listHeaderTextColor
                    font.pointSize: 12
                    font.bold: true
                    width: parent.width - parent.rightPadding - x
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        ListView {
            id: shop_items_table
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height - y

            interactive: true
            clip: true
            focus: true
            keyNavigationEnabled: true
            highlightFollowsCurrentItem: false

            currentIndex: 0
            onModelChanged: currentIndex = 0

            ScrollBar.vertical: ScrollBar {
            }

            model: shopEditorItemModel

            delegate: Rectangle {
                id: delegate_item
                height: shop_items_table.currentIndex == index ? (description_text.lineCount > 1 ? description_text.height + 10 : color_scheme.toolbarHeight) : delegate_row.height + 10
                anchors.left: parent.left
                anchors.right: parent.right
                color: shop_items_table.currentIndex
                       == index ? color_scheme.menuColor : "transparent"

                Row {
                    id: delegate_row
                    padding: 5
                    leftPadding: 10
                    rightPadding: 10
                    spacing: 10
                    anchors.left: parent.left
                    anchors.right: shop_items_table.currentIndex
                                   == index ? delete_item_button.left : parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    height: 30

                    Text {
                        text: modelData.name
                        color: shop_items_table.currentIndex
                               == index ? color_scheme.toolbarTextColor : color_scheme.textColor
                        width: (delegate_item.width - parent.leftPadding * 2
                                - parent.spacing * 2) / 4
                        clip: true
                        elide: Text.ElideRight
                        anchors.verticalCenter: parent.verticalCenter
                        font.pointSize: 12
                    }

                    Text {
                        text: modelData.price
                        color: shop_items_table.currentIndex
                               == index ? color_scheme.toolbarTextColor : color_scheme.textColor
                        width: (delegate_item.width - parent.leftPadding * 2
                                - parent.spacing * 2) / 5
                        clip: true
                        elide: Text.ElideRight
                        horizontalAlignment: Text.AlignRight
                        anchors.verticalCenter: parent.verticalCenter
                        font.pointSize: 12
                    }

                    Text {
                        text: modelData.category
                        color: shop_items_table.currentIndex
                               == index ? color_scheme.toolbarTextColor : color_scheme.textColor
                        width: (delegate_item.width - parent.leftPadding * 2
                                - parent.spacing * 2) / 5
                        clip: true
                        elide: Text.ElideRight
                        anchors.verticalCenter: parent.verticalCenter
                        font.pointSize: 12
                    }

                    Text {
                        id: description_text
                        text: modelData.description
                        color: shop_items_table.currentIndex
                               == index ? color_scheme.toolbarTextColor : color_scheme.textColor
                        width: parent.width - x - parent.rightPadding
                        clip: true
                        elide: Text.ElideRight
                        anchors.verticalCenter: parent.verticalCenter
                        font.pointSize: 12
                        wrapMode: shop_items_table.currentIndex
                                  == index ? Text.WordWrap : Text.NoWrap
                    }
                }

                MouseArea {
                    id: mouse_area
                    anchors.fill: parent
                    onClicked: {
                        shop_items_table.forceActiveFocus()
                        shop_items_table.currentIndex = index
                    }
                }

                ToolBarIconButton {
                    id: delete_item_button
                    fa_icon: FontAwesome.times
                    anchors.top: undefined
                    anchors.bottom: undefined
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    height: color_scheme.toolbarHeight - 10
                    visible: shop_items_table.currentIndex == index
                    onClicked: shop_editor.deleteItem(index)
                }
            }
        }
    }

    // Right
    Column {
        id: right_column
        width: (root.width - left_scroll_view.width - 10) / 2 - item_right_bar.width
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: item_right_bar.left
        anchors.margins: 5
        spacing: 5

        Rectangle {
            id: item_list_header
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
                    width: (parent.width - parent.leftPadding * 2 - parent.spacing) * 2 / 3
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    id: item_header_price_label
                    text: qsTr("Price")
                    color: color_scheme.listHeaderTextColor
                    font.pointSize: 12
                    font.bold: true
                    width: parent.width - x - parent.rightPadding
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: Text.AlignRight
                }
            }
        }

        ListView {
            id: item_list_table
            anchors.left: parent.left
            anchors.right: parent.right

            height: parent.height - parent.spacing - item_list_header.height

            interactive: true
            clip: true
            focus: true
            keyNavigationEnabled: true
            highlightFollowsCurrentItem: false

            currentIndex: 0
            onModelChanged: currentIndex = 0

            ScrollBar.vertical: ScrollBar {
            }

            model: shopEditorItemModel2

            delegate: Rectangle {
                id: item_delegate_rect
                height: item_list_table.currentIndex
                        == index ? Math.max(
                                       item_delegate_text_column.height,
                                       color_scheme.toolbarHeight) : item_delegate_text_column.height
                anchors.left: parent.left
                anchors.right: parent.right
                color: item_list_table.currentIndex
                       == index ? color_scheme.menuColor : "transparent"

                Column {
                    id: item_delegate_text_column
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: item_list_table.currentIndex
                                  == index ? add_item_button.right : parent.left
                    anchors.right: parent.right
                    anchors.margins: 5

                    Item {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: Math.max(30, item_name_text.height,
                                         item_price_text.height)

                        Text {
                            id: item_name_text
                            anchors.left: parent.left
                            anchors.right: item_price_text.left
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter

                            text: modelData.name
                            font.pointSize: 12
                            color: item_list_table.currentIndex
                                   == index ? color_scheme.toolbarTextColor : color_scheme.textColor

                            clip: true
                            elide: Text.ElideRight
                            wrapMode: item_list_table.currentIndex
                                      == index ? Text.Wrap : Text.NoWrap
                        }

                        Text {
                            id: item_price_text
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            width: item_header_price_label.width
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignRight

                            text: modelData.price
                            font.pointSize: 12
                            color: item_list_table.currentIndex
                                   == index ? color_scheme.toolbarTextColor : color_scheme.textColor

                            clip: true
                            elide: Text.ElideRight
                            wrapMode: item_list_table.currentIndex
                                      == index ? Text.WordWrap : Text.NoWrap
                        }
                    }

                    Text {
                        id: item_description_text
                        anchors.left: parent.left
                        anchors.right: parent.right
                        verticalAlignment: Text.AlignTop
                        visible: item_list_table.currentIndex == index
                                 && modelData.description !== ""

                        text: modelData.description
                        font.pointSize: 12
                        color: item_list_table.currentIndex
                               == index ? color_scheme.toolbarTextColor : color_scheme.textColor

                        clip: true
                        elide: Text.ElideRight
                        wrapMode: item_list_table.currentIndex
                                  == index ? Text.WordWrap : Text.NoWrap
                    }

                    Rectangle {
                        visible: item_description_text.visible
                        height: 5
                        width: 5
                        color: "transparent"
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        item_list_table.forceActiveFocus()
                        item_list_table.currentIndex = index
                    }
                }

                ToolBarIconButton {
                    id: add_item_button
                    anchors.top: undefined
                    anchors.bottom: undefined
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.topMargin: 5
                    height: color_scheme.toolbarHeight - 10
                    visible: item_list_table.currentIndex == index

                    fa_icon: FontAwesome.chevronCircleLeft
                    onClicked: shop_editor.addItem(index)
                }
            }
        }
    }

    Item {
        id: item_right_bar
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 5
        width: 175

        Rectangle {
            id: item_category_select_bar
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            color: color_scheme.listHeaderBackgroundColor
            height: color_scheme.toolbarHeight

            ToolBarTextButton {
                labeltext: qsTr("All")
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 5
                width: parent.width / 2 - 10

                textColor: color_scheme.listHeaderTextColor
                hoverColor: "darkgrey"
                onClicked: shop_editor.enableAllItemCategories()
            }

            ToolBarTextButton {
                labeltext: qsTr("None")
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.rightMargin: 5
                width: parent.width / 2 - 10

                textColor: color_scheme.listHeaderTextColor
                hoverColor: "darkgrey"
                onClicked: shop_editor.enableAllItemCategories(false)
            }
        }

        ScrollView {
            id: item_category_scroll_view

            anchors.top: item_category_select_bar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.topMargin: 5
            clip: true

            Column {
                anchors.left: parent.left
                anchors.right: parent.right

                Repeater {
                    id: item_category_repeater
                    model: shop_editor.itemCategories

                    Item {
                        width: item_category_scroll_view.width
                        height: category_checkbox.height

                        CheckBox {
                            id: category_checkbox
                            anchors.left: parent.left
                            checked: true

                            Connections {
                                target: shop_editor
                                onItemGroupChanged: category_checkbox.checked
                                                    = shop_editor.isItemCategoryEnabled(
                                                        modelData)
                            }

                            onClicked: shop_editor.setItemCategoryEnabled(
                                           modelData, checked)
                        }

                        Text {
                            text: modelData
                            color: color_scheme.textColor
                            anchors.left: category_checkbox.right
                            anchors.right: parent.right
                            elide: Text.ElideRight
                            anchors.verticalCenter: parent.verticalCenter
                        }
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
