import QtQuick
import QtQuick.Controls
import src
import "./notes"

Page {
    id: notes_page

    Component.onCompleted: NotesTool.loadData()

    contentItem: SplitView {
        id: split_view
        orientation: Qt.Horizontal

        NotesControlView {
            id: control_view
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            SplitView.minimumWidth: 160
            SplitView.preferredWidth: 200
        }

        Item {
            id: main_item
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            SplitView.minimumWidth: 500
            SplitView.fillWidth: true

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
}
