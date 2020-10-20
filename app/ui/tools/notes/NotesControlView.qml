import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../../defines.js" as Defines
import "../../common"

Rectangle {
    id: root

    color: palette.dark

    CustomButton {
        id: new_notebook_button

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        iconText: FontAwesome.book
        buttonText: qsTr("New Notebook")
        padding: 10

        onClicked: new_notebook_dialog.open()
    }

    Dialog {
        id: new_notebook_dialog
        width: new_notebook_button.width
        height: new_notebook_button.height
        padding: 0
        modal: true

        onOpened: {
            name_field.selectAll()
            name_field.forceActiveFocus()
        }

        onAccepted: {
            notes_tool.createBook(name_field.text)
            name_field.clear()
        }

        contentItem: TextField {
            id: name_field

            placeholderText: qsTr("Notebook")

            onAccepted: {
                new_notebook_dialog.accept()
            }
        }
    }

    TreeView {
        anchors.top: new_notebook_button.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        model: notes_tool.notesModel ? notes_tool.notesModel.childItems : []
        itemIcon: FontAwesome.fileAlt
    }
}
