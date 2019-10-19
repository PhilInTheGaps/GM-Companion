import QtQuick 2.9
import QtQuick.Controls 2.2

import FontAwesome 2.0

Rectangle {
    id: tool_bar

    property alias button_row: row

    property bool enable_back: false
    signal backClicked
    property bool enable_add: false
    signal addClicked
    property bool enable_save: false
    property bool is_saved: true
    signal saveClicked
    property bool enable_export: false
    signal exportClicked

    height: color_scheme.toolbarHeight
    color: color_scheme.toolbarColor

    Row {
        id: row
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        padding: 5
        spacing: 10

        // Back
        ToolBarIconButton {
            visible: enable_back
            fa_icon: FontAwesome.arrowAltCircleLeft
            onClicked: backClicked()
        }

        // Add
        ToolBarIconButton {
            visible: enable_add
            fa_icon: FontAwesome.fileMedical
            onClicked: addClicked()
        }

        // Save
        ToolBarIconButton {
            visible: enable_save
            fa_icon: FontAwesome.fileDownload
            onClicked: saveClicked()

            Text {
                visible: !is_saved
                text: FontAwesome.asterisk
                color: "darkred"
                font.family: FontAwesome.familySolid
                anchors.right: parent.right
                y: parent.height - height * 1.5
            }
        }

        // Export
        ToolBarIconButton {
            visible: enable_export
            fa_icon: FontAwesome.fileExport
            onClicked: exportClicked()
        }
    }
}
