import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0

Dialog {
    id: root
    title: qsTr("New")
    modal: true

    property alias subscenarioChecked: subscenario_check_box.checked
    property alias subscenarioIndex: subscenario_combo_box.currentIndex

    AddonElementDialog {
        id: addon_dialog

        width: editor_root.width * 0.75
        height: editor_root.height * 0.75
        x: (editor_root.width - width) / 2
    }

    contentItem: Column {
        spacing: 10

        Label {
            text: qsTr("Element Type")
        }

        ComboBox {
            id: combo_box
            model: [qsTr("Project"), qsTr("Category"), qsTr("Scenario"), qsTr(
                    "Music List"), qsTr("Sound List"), qsTr("Radio")]
            width: textfield.width
        }

        CustomCheckBox {
            id: subscenario_check_box
            text: (combo_box.currentIndex == 2 ? qsTr("As") : qsTr(
                                                     "In")) + " " + qsTr(
                      "Subscenario")
            checked: false
            visible: combo_box.currentIndex > 1
            anchors.left: parent.left
            anchors.right: parent.right
        }

        ComboBox {
            id: subscenario_combo_box
            model: audio_editor.subscenarioNames

            onModelChanged: console.log(model)

            visible: combo_box.currentIndex > 2 && subscenario_check_box.checked
            anchors.left: parent.left
            anchors.right: parent.right
        }

        Button {
            id: add_from_addons_button

            visible: combo_box.currentIndex > 2

            anchors.left: parent.left
            anchors.right: parent.right
            enabled: combo_box.currentIndex == 6

            text: qsTr("Add from Addons")
            onClicked: addon_dialog.open()
        }

        CustomTextField {
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
                audio_editor.createScenario(textfield.text,
                                            subscenario_check_box.checked)
                break
            case 3:
                audio_editor.createMusicElement(
                            textfield.text, subscenario_check_box.checked,
                            subscenario_combo_box.currentIndex)
                break
            case 4:
                audio_editor.createSoundElement(
                            textfield.text, subscenario_check_box.checked,
                            subscenario_combo_box.currentIndex)
                break
            case 5:
                audio_editor.createRadioElement(
                            textfield.text, subscenario_check_box.checked,
                            subscenario_combo_box.currentIndex)
                break
            }

            textfield.clear()
        }
    }
}
