import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../../../defines.js" as Defines

Item {
    id: root

    signal openEditor

    Component.onCompleted: shop_tool.loadData()

    CategoryList {
        id: category_list
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: Defines.SIDEBAR_WIDTH

        onEditorButtonClicked: openEditor()
    }

    ShopView {
        id: shop_view
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: category_list.right
        anchors.right: shop_list.left
        anchors.leftMargin: 5
        anchors.rightMargin: 5

        visible: shop != undefined

        shop: shop_tool && shop_tool.currentProject
              && shop_tool.currentProject.currentCategory ? shop_tool.currentProject.currentCategory.currentShop : undefined
    }

    ShopList {
        id: shop_list
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: Defines.SIDEBAR_WIDTH
    }
}
