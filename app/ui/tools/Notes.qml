import QtQuick
import QtQuick.Controls
import src
import "./notes"

Page {
    id: notes_page

    Component.onCompleted: NotesTool.loadData()

    NotesControlView {
        id: control_view

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: 200
    }

    Item {
        id: main_item

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: control_view.right
        anchors.right: parent.right

        NotesControlBar {
            id: control_bar

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            textArea: page.textArea
        }

        NotesPage {
            id: page

            anchors.top: control_bar.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
        }
    }
}
