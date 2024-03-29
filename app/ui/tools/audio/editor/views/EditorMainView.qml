import QtQuick
import QtQuick.Controls
import ".."
import "../dialogs"
import "../../../../common"

SplitView {
    id: root

    required property CustomFileDialog fileDialog
    required property EditorDeleteDialog deleteDialog
    required property IconFinderUnsplash unsplashDialog
    required property Dialog largeImageDialog

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

            deleteDialog: root.deleteDialog
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
        SplitView.maximumWidth: root.width / 2

        anchors.top: parent.top
        anchors.bottom: parent.bottom

        fileDialog: root.fileDialog
        unsplashDialog: root.unsplashDialog
        largeImageDialog: root.largeImageDialog
    }
}
