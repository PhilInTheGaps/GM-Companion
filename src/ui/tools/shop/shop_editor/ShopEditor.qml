import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../../../defines.js" as Defines

Page {
    id: root

    signal backToViewer
    signal switchToItemEditor

    Component.onCompleted: shop_editor.loadData()

    Connections {
        target: shop_editor

        onShowInfoBar: {
            info_text.text = message
            info_bar.visible = true
            info_bar_timer.start()
        }
    }

    NewThingDialog {
        id: new_thing_dialog
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
    }

    header: EditorHeader {
        id: tool_bar
    }

    // Shop List
    ShopList {
        id: shop_list
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: 175
    }

    // Shop Info
    ShopInfo {
        id: shop_info
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: item_list.left
        anchors.left: shop_list.right
        anchors.margins: 5
    }

    // Item List
    ItemList {
        id: item_list
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: item_right_bar.left
        anchors.margins: 5
        width: parent.width / 4
    }

    // Item Categories
    Item {
        id: item_right_bar
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 5
        width: 175

        CustomCheckBox {
            id: item_category_select_bar
            text: qsTr("All Categories")
            checked: true

            height: Defines.TOOLBAR_HEIGHT
            anchors.left: parent.left
            anchors.right: parent.right

            color: palette.buttonText
            background: Rectangle {
                color: palette.button
            }

            onClicked: shop_editor.enableAllItemCategories(checked)
        }

        Flickable {
            id: item_category_flickable

            anchors.top: item_category_select_bar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.topMargin: 5
            clip: true
            contentHeight: item_category_column.implicitHeight
            ScrollBar.vertical: CustomScrollBar {
                visible: item_category_flickable.contentHeight > item_category_flickable.height
            }

            Column {
                id: item_category_column
                anchors.left: parent.left
                anchors.right: parent.right

                Repeater {
                    id: item_category_repeater
                    model: shop_editor.itemCategories

                    CustomCheckBox {
                        id: category_checkbox
                        anchors.left: parent.left
                        anchors.right: parent.right

                        text: modelData

                        Connections {
                            target: shop_editor
                            onItemGroupChanged: category_checkbox.checked
                                                = shop_editor.isItemCategoryEnabled(
                                                    modelData)
                        }

                        onClicked: shop_editor.setItemCategoryEnabled(
                                       modelData, checked)
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
        height: Defines.TOOLBAR_HEIGHT
        color: palette.alternateBase
        visible: false

        Timer {
            id: info_bar_timer
            interval: 2000

            onTriggered: {
                info_bar.visible = false
                stop()
            }
        }

        Label {
            id: info_text
            anchors.fill: parent
            anchors.margins: 10
            font.pixelSize: height
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
