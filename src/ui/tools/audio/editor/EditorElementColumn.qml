import QtQuick 2.9
import QtQuick.Controls 2.2

import "../buttons"

ScrollView {
    id: root

    property bool small_mode: true

    anchors.fill: parent
    anchors.margins: 5

    clip: true
    contentWidth: -1
    contentHeight: elements_column.implicitHeight

    Column {
        id: elements_column

        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 5

        Repeater {
            id: repeater
            model: editorElementModel

            EditorElementButton {
                element_name: modelData.name
                element_type: modelData.type
                icon_path: modelData.icon
                has_icon: modelData.hasIcon
                small_mode: root.small_mode

                anchors.left: parent.left
                anchors.right: parent.right
            }
        }
    }
}
