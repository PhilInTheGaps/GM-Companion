import QtQuick
import QtQuick.Controls
import src

Dialog {
    id: root
    title: qsTr("New")
    modal: true

    contentItem: Column {
        spacing: 10

        ComboBox {
            id: combo_box
            anchors.left: parent.left
            anchors.right: parent.right
            model: [qsTr("Project"), qsTr("Category"), qsTr("Shop")]
        }

        TextField {
            id: text_field
            anchors.left: parent.left
            anchors.right: parent.right
            placeholderText: qsTr("Name")
            selectByMouse: true
        }
    }

    standardButtons: Dialog.Ok | Dialog.Cancel

    onAccepted: {
        ShopTool.editor.createThing(text_field.text, combo_box.currentIndex)
        text_field.clear()
    }
}
