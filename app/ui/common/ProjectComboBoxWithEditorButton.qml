import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../defines.js" as Defines

Item {
    id: root

    property alias model: project_combo_box.model
    property alias textRole: project_combo_box.textRole
    property alias emptyString: project_combo_box.emptyString

    signal currentIndexChanged(int index)
    signal editorButtonClicked

    height: Defines.TOOLBAR_HEIGHT
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
        iconText: FontAwesome.edit

        anchors.right: parent.right
        anchors.topMargin: 8
        anchors.bottomMargin: 8

        onClicked: editorButtonClicked()
    }
}
