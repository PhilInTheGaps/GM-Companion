pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import CustomComponents
import src

Rectangle {
    id: root
    color: palette.dark

    Flickable {
        id: shop_scroll_view
        anchors.fill: parent
        anchors.margins: 5

        clip: true
        contentHeight: shop_list_column.implicitHeight
        ScrollBar.vertical: ScrollBar {
            visible: shop_scroll_view.contentHeight > shop_scroll_view.height
        }

        Column {
            id: shop_list_column
            anchors.left: parent.left
            anchors.right: parent.right

            Repeater {
                model: ShopTool.currentProject
                       && ShopTool.currentProject.currentCategory ? ShopTool.currentProject.currentCategory.shops : []

                CustomButton {
                    required property ItemShop modelData

                    buttonText: modelData.name // qmllint disable missing-property
                    width: shop_scroll_view.width

                    onClicked: ShopTool.currentProject.currentCategory.currentShop = modelData
                }
            }
        }
    }
}
