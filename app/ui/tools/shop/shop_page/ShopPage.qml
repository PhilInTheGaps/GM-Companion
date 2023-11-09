import QtQuick
import QtQuick.Controls
import src
import "../../.."

Item {
    id: root

    signal openEditor

    Component.onCompleted: ShopTool.loadData()

    SplitView {
        id: split_view

        orientation: Qt.Horizontal
        anchors.fill: parent

        CategoryList {
            id: category_list
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left

            SplitView.minimumWidth: 160
            SplitView.preferredWidth: Sizes.sidebarWidth

            onEditorButtonClicked: root.openEditor()
        }

        ShopView {
            id: shop_view
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            SplitView.minimumWidth: 250
            SplitView.fillWidth: true

            visible: shop != undefined

            shop: ShopTool.currentProject
                  && ShopTool.currentProject.currentCategory ? ShopTool.currentProject.currentCategory.currentShop : undefined
        }

        PageShopList {
            id: shop_list
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            SplitView.minimumWidth: 160
            SplitView.preferredWidth: Sizes.sidebarWidth
        }
    }
}
