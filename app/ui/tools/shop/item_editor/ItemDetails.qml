import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0

Rectangle {
    id: root
    color: palette.dark

    Column {
        id: left_column
        anchors.fill: parent
        anchors.margins: 5
        spacing: 5

        TextField {
            id: category_name_field
            anchors.left: parent.left
            anchors.right: parent.right
            placeholderText: qsTr("Category Name")
            selectByMouse: true
        }

        Button {
            id: add_category_button
            anchors.left: parent.left
            anchors.right: parent.right
            text: qsTr("Add Category")

            onClicked: {
                if (category_name_field.text != "") {
                    shop_tool.editor.itemEditor.addCategory(
                                category_name_field.text)
                }
            }
        }

        TextField {
            id: item_name_field
            anchors.left: parent.left
            anchors.right: parent.right
            placeholderText: qsTr("Item Name")
            selectByMouse: true
            enabled: category_combobox.currentText != ""
        }

        TextField {
            id: item_price_field
            anchors.left: parent.left
            anchors.right: parent.right
            placeholderText: qsTr("Price")
            selectByMouse: true
            enabled: category_combobox.currentText != ""
        }

        ComboBox {
            id: category_combobox
            anchors.left: parent.left
            anchors.right: parent.right
            model: shop_tool && shop_tool.editor
                   && shop_tool.editor.itemEditor ? shop_tool.editor.itemEditor.categories : []
        }

        CustomTextEdit {
            id: item_description_field
            anchors.left: parent.left
            anchors.right: parent.right
            height: width

            enabled: category_combobox.currentText != ""
            placeholderText: qsTr("Item Description")
        }

        Button {
            anchors.left: parent.left
            anchors.right: parent.right
            text: qsTr("Add Item")
            enabled: category_combobox.currentText != ""

            onClicked: shop_tool.editor.itemEditor.addItem(
                           item_name_field.text, item_price_field.text,
                           category_combobox.currentText,
                           item_description_field.text)
        }
    }
}
