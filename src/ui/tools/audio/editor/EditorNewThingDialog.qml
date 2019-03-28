import QtQuick 2.9
import QtQuick.Controls 2.2

Dialog {
    id: root
    title: qsTr("New")
    modal: true

    AddonElementDialog {
        id: addon_dialog

        width: editor_root.width * 0.75
        height: editor_root.height * 0.75
        x: (editor_root.width - width) / 2

        onAddAddonElements: {
            for (var i = 0; i < names.length; i++) {
                if (checked[i]) {
                    audio_editor.createSpotifyElement(names[i])
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
            model: [qsTr("Project"), qsTr("Category"), qsTr("Scenario"), qsTr(
                    "Music List"), qsTr("Sound List"), qsTr("Radio"), qsTr(
                    "Spotify Playlist")]
            width: textfield.width
        }

        Button {
            id: add_from_addons_button

            anchors.left: parent.left
            anchors.right: parent.right
            enabled: combo_box.currentIndex == 6

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
            switch (combo_box.currentIndex) {
            case 0:
                audio_editor.createProject(textfield.text)
                break
            case 1:
                audio_editor.createCategory(textfield.text)
                break
            case 2:
                audio_editor.createScenario(textfield.text)
                break
            case 3:
                audio_editor.createMusicElement(textfield.text)
                break
            case 4:
                audio_editor.createSoundElement(textfield.text)
                break
            case 5:
                audio_editor.createRadioElement(textfield.text)
                break
            case 6:
                audio_editor.createSpotifyElement(textfield.text)
                break
            }

            textfield.clear()
        }
    }
}
