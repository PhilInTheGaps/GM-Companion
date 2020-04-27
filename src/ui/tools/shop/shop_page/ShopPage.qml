import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../../../defines.js" as Defines

Item {
    id: root

    readonly property int sidebarWidth: 180
    signal openEditor

    Component.onCompleted: shop_tool.loadData()

    CategoryList {
        id: category_list
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: sidebarWidth

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
    }

    ShopList {
        id: shop_list
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: sidebarWidth
    }
}
