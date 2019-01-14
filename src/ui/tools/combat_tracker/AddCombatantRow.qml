import QtQuick 2.9
import QtQuick.Controls 2.2

Row {
    id: row
    spacing: 5
    anchors.left: parent.left
    anchors.right: parent.right

    signal copyPrevious

    function setCombatant(name, ini, health) {
        add_name_field.text = name
        add_ini_spinbox.value = ini
        add_health_spinbox.value = health
    }

    function clear() {
        add_name_field.text = ""
        add_ini_spinbox.value = 0
        add_health_spinbox.value = 0
    }

    function getName() {
        return add_name_field.text
    }

    function getIni() {
        return add_ini_spinbox.value
    }

    function getHealth() {
        return add_health_spinbox.value
    }

    Item {
        width: (parent.width - parent.spacing * 3) / 4
        height: copy_button.height

        Button {
            id: copy_button
            text: qsTr("Copy Down")
            anchors.right: parent.right
            width: parent.width / 2
            onClicked: copyPrevious()
        }
    }

    TextField {
        id: add_name_field
        width: (parent.width - parent.spacing * 3) / 4
        placeholderText: qsTr("Name")
        selectByMouse: true
    }

    SpinBox {
        id: add_ini_spinbox
        width: (parent.width - parent.spacing * 3) / 4
        editable: true

        from: 0
        to: 99999
    }

    SpinBox {
        id: add_health_spinbox
        width: (parent.width - parent.spacing * 3) / 4
        editable: true

        from: 0
        to: 99999
    }
}
