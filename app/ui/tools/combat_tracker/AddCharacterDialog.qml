import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../../defines.js" as Defines

Dialog {
    id: add_dialog

    modal: true

    function addCombatants() {
        if (name_textfield.text != "") {
            combat_tracker.add(name_textfield.text, ini_spinbox.value,
                               health_spinbox.value, priority_spinbox.value,
                               notes_textfield.text)
        }
    }

    function clearAddFields() {
        name_textfield.clear()
        ini_spinbox.value = 0
        health_spinbox.value = 0
        priority_spinbox.value = 0
        notes_textfield.clear()
    }

    onVisibleChanged: {
        if (visible) {
            name_textfield.forceActiveFocus()
        }
    }

    padding: 0

    contentItem: Grid {
        id: main_item
        height: Defines.TOOLBAR_HEIGHT

        spacing: 10
        padding: 10
        columns: 6
        rows: 1

        function addCombatant() {
            add_dialog.addCombatants()
            add_dialog.clearAddFields()
            name_textfield.forceActiveFocus()
        }

        TextField {
            id: name_textfield
            width: parent.width / 5
            selectByMouse: true

            placeholderText: qsTr("Name")

            onAccepted: {
                main_item.addCombatant()
            }
        }

        SpinBox {
            id: ini_spinbox
            height: name_textfield.height
            width: list_view.width / 6
            from: -99999
            to: 99999
            editable: true
        }

        SpinBox {
            id: health_spinbox
            height: name_textfield.height
            width: list_view.width / 6
            from: -99999
            to: 99999
            editable: true
        }

        SpinBox {
            id: priority_spinbox
            height: name_textfield.height
            width: list_view.width / 6
            from: -99999
            to: 99999
            editable: true
        }

        TextField {
            id: notes_textfield
            width: parent.width - x - add_button.width - main_item.spacing - main_item.padding

            onAccepted: {
                main_item.addCombatant()
            }
        }

        Button {
            id: add_button
            text: FontAwesome.plus
            font.family: FontAwesome.familySolid

            height: name_textfield.height
            width: height

            font.pointSize: 16

            onClicked: {
                add_dialog.addCombatants()
                add_dialog.clearAddFields()
                add_dialog.visible = false
            }
        }
    }
}
