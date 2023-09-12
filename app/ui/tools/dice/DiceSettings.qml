import QtQuick
import QtQuick.Controls
import src

Dialog {
    id: root
    title: qsTr("Dice Settings")
    modal: true

    contentItem: Column {
        padding: 5
        spacing: 5

        CheckBox {
            id: enable_criticals_checkbox
            text: qsTr("Enable Criticals")
            checked: DiceTool.getCriticalEnabled()
        }

        RadioButton {
            id: min_max_radiobutton
            text: qsTr("Min / Max")
            checked: DiceTool.getMinMax()
            enabled: enable_criticals_checkbox.checked
        }

        Column {
            padding: 10
            spacing: 5

            RadioButton {
                id: success_max_radiobutton
                text: qsTr("Success: Maximum, Failure: Minimum")
                checked: DiceTool.getSuccessMax()
                enabled: enable_criticals_checkbox.checked
            }

            RadioButton {
                text: qsTr("Success: Minimum, Failure: Maximum")
                checked: !DiceTool.getSuccessMax()
                enabled: enable_criticals_checkbox.checked
            }
        }

        RadioButton {
            text: qsTr("Fixed Values")
            checked: !DiceTool.getMinMax()
            enabled: enable_criticals_checkbox.checked
        }

        Column {
            padding: 10
            spacing: 5

            Label {
                text: qsTr("Critical Success") + ":"
            }

            SpinBox {
                id: success_spinbox
                from: 0
                to: 1000
                value: DiceTool.getSuccess()
                editable: true
                enabled: enable_criticals_checkbox.checked
            }

            Label {
                text: qsTr("Critical Failure") + ":"
            }

            SpinBox {
                id: failure_spinbox
                from: 0
                to: 1000
                value: DiceTool.getFailure()
                editable: true
                enabled: enable_criticals_checkbox.checked
            }
        }
    }

    standardButtons: Dialog.Ok | Dialog.Cancel

    onAccepted: {
        DiceTool.setDiceSettings(enable_criticals_checkbox.checked,
                                  success_spinbox.value, failure_spinbox.value,
                                  min_max_radiobutton.checked,
                                  success_max_radiobutton.checked)
    }
}
