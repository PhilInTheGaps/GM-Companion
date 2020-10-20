import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../../../defines.js" as Defines

Rectangle {
    id: left_menu
    color: palette.dark

    signal editorButtonClicked

    Item {
        id: top_bar

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: Defines.TOOLBAR_HEIGHT

        anchors.leftMargin: 5
        anchors.rightMargin: 5

        // Project ComboBox
        CustomToolBarComboBox {
            anchors.left: parent.left
            anchors.right: editor_button.left
            width: parent.width - editor_button.width - parent.spacing
            leftPadding: 0
            model: shop_tool.projects

            onCurrentTextChanged: shop_tool.setCurrentProject(currentIndex)
        }

        // Open Editor Button
        CustomToolBarButton {
            id: editor_button
            iconText: FontAwesome.edit

            anchors.right: parent.right
            anchors.topMargin: 8
            anchors.bottomMargin: 8

            onClicked: editorButtonClicked()
        }
    }

    ScrollView {
        anchors.top: top_bar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 5

        clip: true
        contentWidth: -1
        contentHeight: category_column.implicitHeight

        Column {
            id: category_column
            anchors.left: parent.left
            anchors.right: parent.right

            Repeater {
                model: shop_tool.categories
                onModelChanged: console.debug(model)

                CustomButton {
                    buttonText: modelData

                    anchors.left: parent.left
                    anchors.right: parent.right

                    onClicked: shop_tool.setCurrentCategory(index)
                }
            }
        }
    }
}
