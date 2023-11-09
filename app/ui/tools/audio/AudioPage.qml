import QtQuick
import QtQuick.Controls
import src

Page {
    id: root

    readonly property int sidebarWidth: 180
    signal openEditor

    Component.onCompleted: AudioTool.loadData()

    footer: AudioControlBar {
        onTogglePlaylist: {
            info_view.visible = !info_view.visible;
        }
    }

    contentItem: SplitView {
        id: split_view
        orientation: Qt.Horizontal

        AudioCategoryView {
            id: category_view

            SplitView.minimumWidth: 160
            SplitView.preferredWidth: root.sidebarWidth

            anchors.top: parent.top
            anchors.bottom: parent.bottom

            onEditorButtonClicked: root.openEditor()
        }

        AudioElementView {
            id: element_view

            SplitView.minimumWidth: 250
            SplitView.fillWidth: true

            anchors.top: parent.top
            anchors.bottom: parent.bottom
        }

        AudioInfoView {
            id: info_view

            SplitView.minimumWidth: 160
            SplitView.preferredWidth: root.sidebarWidth

            anchors.top: parent.top
            anchors.bottom: parent.bottom
        }
    }
}
