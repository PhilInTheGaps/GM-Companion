import QtQuick 2.9
import QtQuick.Controls 2.2

Column {
    id: column

    property bool enable_move_buttons: true
    signal moveUp
    signal moveDown

    property bool enable_delete: true
    signal deleteClicked

    anchors.left: parent.left
    anchors.right: parent.right
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
