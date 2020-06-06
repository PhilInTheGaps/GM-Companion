import QtQuick 2.6
import QtQuick.Controls 2.2

Page {
    id: root

    readonly property int sidebarWidth: 180
    signal openEditor

    footer: AudioControlBar {}

    Component.onCompleted: audio_tool.loadData()

    AudioCategoryView {
        id: category_view

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: sidebarWidth

        onEditorButtonClicked: openEditor()
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
        width: sidebarWidth
    }
}
