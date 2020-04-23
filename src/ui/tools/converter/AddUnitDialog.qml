import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0

Dialog {
    id: root
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    title: qsTr("Add Units")
    modal: true

    contentItem: Column {
        spacing: 5

        ComboBox {
            id: unit_type
            model: ["Length", "Area", "Volume", "Weight", "Money"]

            onCurrentTextChanged: refUnit_text.text = converter_tool.refUnitName(
                                      currentText)
        }

        Grid {
            spacing: 5
            columns: 2

            Label {
                text: qsTr("Unit Name:")
            }

            Label {
                id: refUnit_text
                text: qsTr("Meters")
            }

            CustomTextField {
                id: unit_name
                selectByMouse: true
            }

            CustomTextField {
                id: unit_value
                selectByMouse: true
            }
        }

        Button {
            text: qsTr("Add Unit")

            onClicked: converter_tool.addUnit(unit_name.text, unit_value.text,
                                              unit_type.currentText)
        }
    }

    standardButtons: Dialog.Close
}
