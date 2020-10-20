import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../../defines.js" as Defines

Control {
    id: add_rect

    function addCombatants() {
        if (name_textfield.text != "") {
            combat_tracker.add(name_textfield.text, ini_spinbox.value,
                               health_spinbox.value, notes_textfield.text)
        }
    }

    function clearAddFields() {
        name_textfield.clear()
        ini_spinbox.value = 0
        health_spinbox.value = 0
        notes_textfield.clear()
    }

    onVisibleChanged: {
        if (visible) {
            name_textfield.forceActiveFocus()
        }
    }

    contentItem: Grid {
        id: main_item
        height: Defines.TOOLBAR_HEIGHT

        spacing: 10
        padding: 10
        columns: 4
        rows: 1

        TextField {
            id: name_textfield
            width: parent.width / 5
            selectByMouse: true

            placeholderText: qsTr("Name")

            onAccepted: {
                add_rect.addCombatants()
                add_rect.clearAddFields()
            }
        }

        SpinBox {
            id: ini_spinbox
            height: name_textfield.height
            width: list_view.width / 6
            from: 0
            to: 99999
            editable: true
        }

        SpinBox {
            id: health_spinbox
            height: name_textfield.height
            width: list_view.width / 6
            from: 0
            to: 99999
            editable: true
        }

        TextField {
            id: notes_textfield
            width: parent.width - x - add_button.width - main_item.spacing

            onAccepted: {
                add_rect.addCombatants()
                add_rect.clearAddFields()
                name_textfield.forceActiveFocus()
            }
        }
    }

    CustomToolBarButton {
        id: add_button
        iconText: FontAwesome.plus
        transparentBackground: true

        anchors.top: main_item.top
        anchors.bottom: main_item.bottom
        anchors.right: main_item.right
        anchors.margins: 10
        anchors.rightMargin: 0

        width: height
        centering: true
        pointSize: 18

        onClicked: {
            add_rect.addCombatants()
            add_rect.clearAddFields()
            add_rect.visible = false
        }
    }
}
