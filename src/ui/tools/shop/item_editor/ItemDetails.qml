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

        CustomTextField {
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
                    item_editor.addCategory(category_name_field.text)
                }
            }
        }

        CustomTextField {
            id: item_name_field
            anchors.left: parent.left
            anchors.right: parent.right
            placeholderText: qsTr("Item Name")
            selectByMouse: true
            enabled: category_combobox.currentText != ""
        }

        CustomTextField {
            id: item_price_field
            anchors.left: parent.left
            anchors.right: parent.right
            placeholderText: qsTr("Price")
            selectByMouse: true
            enabled: category_combobox.currentText != ""
        }

        CustomComboBox {
            id: category_combobox
            anchors.left: parent.left
            anchors.right: parent.right
            model: item_editor.categories
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

            onClicked: item_editor.addItem(item_name_field.text,
                                           item_price_field.text,
                                           category_combobox.currentText,
                                           item_description_field.text)
        }
    }
}
