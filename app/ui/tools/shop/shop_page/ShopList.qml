import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0

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
                model: shop_tool && shop_tool.currentProject
                       && shop_tool.currentProject.currentCategory ? shop_tool.currentProject.currentCategory.shops : []

                CustomButton {
                    buttonText: modelData.name
                    width: shop_scroll_view.width

                    onClicked: shop_tool.currentProject.currentCategory.currentShop = modelData
                }
            }
        }
    }
}
