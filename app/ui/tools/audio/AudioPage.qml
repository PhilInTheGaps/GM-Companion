import QtQuick
import QtQuick.Controls
import src

Page {
    id: root

    readonly property int sidebarWidth: 180
    signal openEditor

    footer: AudioControlBar {
        onTogglePlaylist: {
            info_view.visible = !info_view.visible
        }
    }

    Component.onCompleted: AudioTool.loadData()

    AudioCategoryView {
        id: category_view

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: root.sidebarWidth

        onEditorButtonClicked: root.openEditor()
    }

    AudioElementView {
        id: element_view

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: category_view.right
        anchors.right: info_view.visible ? info_view.left : parent.right
    }

    AudioInfoView {
        id: info_view

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: root.sidebarWidth
    }
}
