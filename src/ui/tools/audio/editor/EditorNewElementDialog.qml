import QtQuick 2.9
import QtQuick.Controls 2.2

Dialog {
    id: root
    title: qsTr("Create New Element")

    contentItem: Column {
        spacing: 5

        Text {
            text: qsTr("Element Type")
        }

        ComboBox {
            id: combo_box
            model: [qsTr("Music List"), qsTr("Sound List"), qsTr("Radio"), qsTr(
                    "Spotify Playlist")]
            width: textfield.width
        }

        TextField {
            id: textfield
            placeholderText: qsTr("Element Name")
            selectByMouse: true
        }
    }

    standardButtons: Dialog.Ok | Dialog.Cancel

    onAccepted: {
        if (textfield.text != "") {
            editor_tool.createList(textfield.text, combo_box.currentIndex)
            textfield.clear()
        }
    }
}
