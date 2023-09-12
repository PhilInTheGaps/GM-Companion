import QtQuick
import QtQuick.Controls
import IconFonts
import src
import "../.."

Dialog {
    id: root

    property double spinboxWidth

    modal: true

    function addCombatants() {
        if (name_textfield.text !== "") {
            CombatTrackerTool.add(name_textfield.text, ini_spinbox.value,
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
        height: Sizes.toolbarHeight

        spacing: 10
        padding: 10
        columns: 6
        rows: 1

        function addCombatant() {
            root.addCombatants()
            root.clearAddFields()
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
            width: root.spinboxWidth
            from: -99999
            to: 99999
            editable: true
        }

        SpinBox {
            id: health_spinbox
            height: name_textfield.height
            width: root.spinboxWidth
            from: -99999
            to: 99999
            editable: true
        }

        SpinBox {
            id: priority_spinbox
            height: name_textfield.height
            width: root.spinboxWidth
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
            font.family: FontAwesome.fontSolid.family
            font.styleName: FontAwesome.fontSolid.styleName

            height: name_textfield.height
            width: height

            font.pointSize: 16

            onClicked: {
                root.addCombatants()
                root.clearAddFields()
                root.visible = false
            }
        }
    }
}
