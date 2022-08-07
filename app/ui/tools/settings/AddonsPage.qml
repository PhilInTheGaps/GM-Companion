import QtQuick 2.9
import QtQuick.Controls 2.2

Item {
    id: root

    Flickable {
        id: flickable
        anchors.fill: parent
        anchors.margins: 10
        anchors.rightMargin: 0

        clip: true
        contentHeight: addon_column.implicitHeight
        contentWidth: -1
        interactive: true

        ScrollBar.vertical: ScrollBar {
            id: scroll_bar
            visible: flickable.contentHeight > flickable.height
        }

        Column {
            id: addon_column
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 10
            spacing: 10

            Repeater {
                model: addon_manager.addons

                AddonItem {
                    addon: modelData
                    addon_index: index

                    anchors.left: parent.left
                    anchors.right: parent.right
                }
            }
        }
    }
}
