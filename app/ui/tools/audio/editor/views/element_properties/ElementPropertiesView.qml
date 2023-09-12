import QtQuick
import QtQuick.Controls
import "../.."
import "../../../../../common"

Column {
    id: root

    required property CustomFileDialog fileDialog
    required property IconFinderUnsplash unsplashDialog
    required property Dialog largeImageDialog

    property bool enableMoveButtons: true
    signal moveUp
    signal moveDown

    property bool enableDelete: true
    signal deleteClicked

    anchors.left: parent.left
    anchors.right: parent.right
    spacing: 5

    // Name, move, delete
    ElementNameView {
        id: row0
        enableMoveButtons: root.enableMoveButtons
        enableDelete: root.enableDelete

        onDeleteClicked: root.deleteClicked()
    }

    // Icon
    ElementIconView {
        id: row1

        fileDialog: root.fileDialog
        unsplashDialog: root.unsplashDialog // qmllint disable incompatible-type
        largeImageDialog: root.largeImageDialog
    }

    // Playback Mode
    PlaybackModeView {
        id: row2
    }
}
