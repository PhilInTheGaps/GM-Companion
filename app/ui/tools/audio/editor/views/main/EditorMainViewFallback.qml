import QtQuick 2.9

import ".."
import "../.."

Item {
    id: main_item
    anchors.fill: parent

    Item {
        id: left_item
        width: 220

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        EditorElementColumn {
            id: element_column
        }
    }

    EditorFileListView {
        id: mid_item

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: left_item.right
        anchors.right: right_item.left
    }

    EditorRightColumn {
        id: right_item

        width: 300
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
    }
}
