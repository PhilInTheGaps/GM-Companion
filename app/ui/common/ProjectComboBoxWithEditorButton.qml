import QtQuick
import CustomComponents
import IconFonts
import ".."

Item {
    id: root

    property alias model: project_combo_box.model
    property alias textRole: project_combo_box.textRole
    property alias emptyString: project_combo_box.emptyString

    signal currentIndexChanged(int index)
    signal editorButtonClicked

    height: Sizes.toolbarHeight
    anchors.leftMargin: 5
    anchors.rightMargin: 5

    // Project ComboBox
    CustomToolBarComboBox {
        id: project_combo_box

        anchors.left: parent.left
        anchors.right: editor_button.left

        width: parent.width - editor_button.width

        textRole: "name"
        emptyString: ""

        onCurrentIndexChanged: root.currentIndexChanged(currentIndex)
    }

    // Open Editor Button
    CustomToolBarButton {
        id: editor_button
        iconText: FontAwesome.penToSquare

        anchors.right: parent.right
        anchors.topMargin: 8
        anchors.bottomMargin: 8

        onClicked: root.editorButtonClicked()
    }
}
