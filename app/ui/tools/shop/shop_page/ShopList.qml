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
            spacing: 5

            Repeater {
                model: shop_tool.shops

                CustomButton {
                    buttonText: modelData
                    width: shop_scroll_view.width

                    onClicked: shop_tool.setCurrentShop(index)
                }
            }
        }
    }
}
