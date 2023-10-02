import QtQuick.Controls

Page {
    id: root
    signal openEditor

    ConverterMenu {
        id: side_bar
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        onOpenEditor: root.openEditor()
    }

    ConverterUnitPage {
        id: unit_page
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: side_bar.right
        anchors.right: parent.right
    }
}