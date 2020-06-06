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
        ScrollBar.vertical: CustomScrollBar {
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
                model: shop_editor.shopNames

                CustomButton {
                    buttonText: modelData
                    anchors.left: parent.left
                    anchors.right: parent.right
                    onClicked: shop_editor.setCurrentShop(index)
                }
            }
        }
    }
}
