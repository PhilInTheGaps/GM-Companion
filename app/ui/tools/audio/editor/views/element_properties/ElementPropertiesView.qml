import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0

import "../../../../../defines.js" as Defines

Rectangle {
    property bool enable_move_buttons: true
    signal moveUp
    signal moveDown

    property bool enable_delete: true
    signal deleteClicked

    anchors.left: parent.left
    anchors.right: parent.right
    height: column.height
    color: "transparent"

    Column {
        id: column
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        spacing: 5

        // Name, move, delete
        ElementNameView {
            id: row0
        }

        // Icon
        ElementIconView {
            id: row1
        }

        // Playback Mode
        PlaybackModeView {
            id: row2
        }
    }
}
