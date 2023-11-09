import QtQuick.Controls
import "../.."

Page {
    id: root
    signal openEditor

    contentItem: SplitView {
        id: split_view
        orientation: Qt.Horizontal

        ConverterMenu {
            id: side_bar
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            SplitView.minimumWidth: 160
            SplitView.preferredWidth: Sizes.sidebarWidth

            onOpenEditor: root.openEditor()
        }

        ConverterUnitPage {
            id: unit_page
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            SplitView.minimumWidth: 500
            SplitView.fillWidth: true
        }
    }
}
