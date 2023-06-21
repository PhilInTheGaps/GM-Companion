import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0

Rectangle {
    id: left_item
    color: palette.dark

    Flickable {
        id: shop_flickable
        anchors.fill: parent

        clip: true
        contentHeight: shop_column.implicitHeight
        ScrollBar.vertical: ScrollBar {
            id: scroll_bar
            visible: shop_flickable.contentHeight > shop_flickable.height
        }

        Column {
            id: shop_column
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 5
            anchors.rightMargin: scroll_bar.visible ? scroll_bar.width : 5
            spacing: 5
            topPadding: 5

            Repeater {
                model: shop_tool && shop_tool.editor
                       && shop_tool.editor.currentProject
                       && shop_tool.editor.currentProject.currentCategory ? shop_tool.editor.currentProject.currentCategory.shops : []

                CustomButton {
                    buttonText: modelData.name
                    anchors.left: parent.left
                    anchors.right: parent.right
                    onClicked: shop_tool.editor.currentProject.currentCategory.currentShop
                               = modelData
                }
            }
        }
    }
}
