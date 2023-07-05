import QtQuick
import QtQuick.Controls

import ".."

SplitView {
    id: main_item
    anchors.fill: parent
    orientation: Qt.Horizontal

    Item {
        id: left_item
        SplitView.minimumWidth: 180
        SplitView.preferredWidth: 220

        anchors.top: parent.top
        anchors.bottom: parent.bottom

        EditorElementColumn {
            id: element_column
        }
    }

    EditorFileListView {
        id: mid_item
        SplitView.minimumWidth: 250
        SplitView.fillWidth: true

        anchors.top: parent.top
        anchors.bottom: parent.bottom
    }

    EditorRightColumn {
        id: right_item

        SplitView.minimumWidth: 180
        SplitView.preferredWidth: 300
        SplitView.maximumWidth: main_item.width / 2

        anchors.top: parent.top
        anchors.bottom: parent.bottom
    }
}
