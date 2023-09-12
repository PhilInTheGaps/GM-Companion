import QtQuick
import src
import "../../.."

Item {
    id: root

    signal openEditor

    Component.onCompleted: ShopTool.loadData()

    CategoryList {
        id: category_list
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: Sizes.sidebarWidth

        onEditorButtonClicked: root.openEditor()
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

        shop: ShopTool.currentProject
              && ShopTool.currentProject.currentCategory ? ShopTool.currentProject.currentCategory.currentShop : undefined
    }

    PageShopList {
        id: shop_list
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: Sizes.sidebarWidth
    }
}
