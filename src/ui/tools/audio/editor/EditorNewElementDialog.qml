import QtQuick 2.9
import QtQuick.Controls 2.2

Dialog {
    id: root
    title: qsTr("Create New Element")

    AddonElementDialog {
        id: addon_dialog

        width: audio_editor.width * 0.75
        height: audio_editor.height * 0.75
        x: (audio_editor.width - width) / 2

        onAddAddonElements: {
            for (var i = 0; i < names.length; i++) {
                if (checked[i]) {
                    editor_tool.createList(names[i], combo_box.currentIndex,
                                           [uris[i]])
                }
            }
        }
    }

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

        Button {
            id: add_from_addons_button

            anchors.left: parent.left
            anchors.right: parent.right
            enabled: combo_box.currentIndex == 3

            text: qsTr("Add from Addons")
            onClicked: addon_dialog.open()
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
