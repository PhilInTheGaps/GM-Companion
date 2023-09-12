import QtQuick
import QtQuick.Controls
import CustomComponents
import src

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
                model: ShopTool && ShopTool.editor
                       && ShopTool.editor.currentProject
                       && ShopTool.editor.currentProject.currentCategory ? ShopTool.editor.currentProject.currentCategory.shops : []

                CustomButton {
                    required property ItemShop modelData

                    buttonText: modelData.name // qmllint disable missing-property
                    anchors.left: parent.left
                    anchors.right: parent.right
                    onClicked: ShopTool.editor.currentProject.currentCategory.currentShop
                               = modelData
                }
            }
        }
    }
}
