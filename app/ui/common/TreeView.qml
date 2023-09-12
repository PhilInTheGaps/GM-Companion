pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import common

ListView {
    id: root

    property bool useTypeIcons: true
    property string itemIcon: ""

    ScrollBar.vertical: ScrollBar {
        id: scrollbar
        anchors.right: root.right
        visible: root.contentHeight > root.height
    }

    clip: true

    delegate: Column {
        id: column
        required property TreeItem modelData

        anchors.left: parent ? parent.left : undefined
        anchors.right: parent ? parent.right : undefined
        anchors.rightMargin: scrollbar.visible ? scrollbar.width : anchors.margins

        TreeViewItem {
            modelData: column.modelData
            itemIcon: root.itemIcon
        }
    }
}
