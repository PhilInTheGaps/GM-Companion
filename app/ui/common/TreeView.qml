import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../defines.js" as Defines

ListView {
    id: root

    property bool useTypeIcons: true
    property string itemIcon: ""

    ScrollBar.vertical: ScrollBar {
        id: scrollbar
        anchors.right: root.right
        visible: root.contentHeight > root.height
    }

    //    spacing: 5
    clip: true

    delegate: Column {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: scrollbar.visible ? scrollbar.width : anchors.margins

        TreeViewItem {
            itemIcon: root.itemIcon
        }
    }
}
