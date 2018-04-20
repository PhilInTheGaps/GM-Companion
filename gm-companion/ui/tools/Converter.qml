import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4

import gm.companion.convertertool 1.0

Page {
    id: converter

    ConverterTool {
        id: converter_tool

        onLengthUnitsChanged: {
            length_units_1.model = lengthUnits
            length_units_2.model = lengthUnits
        }

        onAreaUnitsChanged: {
            area_units_1.model = areaUnits
            area_units_2.model = areaUnits
        }

        onVolumeUnitsChanged: {
            volume_units_1.model = volumeUnits
            volume_units_2.model = volumeUnits
        }

        onWeightUnitsChanged: {
            weight_units_1.model = weightUnits
            weight_units_2.model = weightUnits
        }

        onMoneyUnitsChanged: {
            money_units_1.model = moneyUnits
            money_units_2.model = moneyUnits
        }
    }

    ScrollView {
        width: parent.width
        height: parent.height
        clip: true

        Column {
            width: parent.width
            spacing: 10
            padding: 5

            Dialog {
                id: add_unit_dialog

                title: qsTr("Add Units")

                contentItem: Column {
                    spacing: 5

                    ComboBox {
                        id: unit_type
                        model: ["Length", "Area", "Volume", "Weight", "Money"]

                        onCurrentTextChanged: refUnit_text.text = converter_tool.refUnitName(
                                                  currentText)
                    }

                    Row {
                        spacing: 5

                        Grid {
                            spacing: 5
                            columns: 2

                            Text {
                                text: qsTr("Unit Name:")
                            }

                            Text {
                                id: refUnit_text
                                text: "Meters"
                            }

                            TextField {
                                id: unit_name
                            }

                            TextField {
                                id: unit_value
                            }
                        }
                    }

                    Button {
                        text: qsTr("Add Unit")

                        onClicked: converter_tool.addUnit(unit_name.text,
                                                          unit_value.text,
                                                          unit_type.currentText)
                    }
                }

                standardButtons: Dialog.Close
            }

            Column {
                spacing: 5

                Button {
                    text: qsTr("Add Custom Units")

                    onClicked: add_unit_dialog.open()
                }

                Text {
                    text: qsTr("Length")
                }

                Row {
                    id: length_row_1
                    width: parent.width
                    spacing: 5

                    TextField {
                        id: length_units_text_1
                        selectByMouse: true

                        onTextChanged: {
                            length_output.text = converter_tool.convertLength(
                                        length_units_1.currentText,
                                        length_units_2.currentText, text)
                        }
                    }

                    ComboBox {
                        id: length_units_1
                        width: 200
                        model: converter_tool.lengthUnits

                        onCurrentTextChanged: {
                            length_output.text = converter_tool.convertLength(
                                        currentText,
                                        length_units_2.currentText,
                                        length_units_text_1.text)
                        }
                    }
                }

                Row {
                    id: length_row_2
                    width: parent.width
                    spacing: 5

                    TextField {
                        id: length_output
                        readOnly: true
                        selectByMouse: true
                    }

                    ComboBox {
                        id: length_units_2
                        width: 200
                        model: converter_tool.lengthUnits

                        onCurrentTextChanged: {
                            length_output.text = converter_tool.convertLength(
                                        length_units_1.currentText,
                                        currentText, length_units_text_1.text)
                        }
                    }
                }

                Text {
                    text: qsTr("Area")
                }

                Row {
                    id: area_row_1
                    width: parent.width
                    spacing: 5

                    TextField {
                        id: area_units_text_1
                        selectByMouse: true

                        onTextChanged: {
                            area_output.text = converter_tool.convertArea(
                                        area_units_1.currentText,
                                        area_units_2.currentText, text)
                        }
                    }

                    ComboBox {
                        id: area_units_1
                        width: 200

                        model: converter_tool.areaUnits
                        onCurrentTextChanged: {
                            area_output.text = converter_tool.convertArea(
                                        currentText, area_units_2.currentText,
                                        area_units_text_1.text)
                        }
                    }
                }

                Row {
                    id: area_row_2
                    width: parent.width
                    spacing: 5

                    TextField {
                        id: area_output
                        readOnly: true
                        selectByMouse: true
                    }

                    ComboBox {
                        id: area_units_2
                        width: 200

                        model: converter_tool.areaUnits
                        onCurrentTextChanged: {
                            area_output.text = converter_tool.convertArea(
                                        area_units_1.currentText, currentText,
                                        area_units_text_1.text)
                        }
                    }
                }

                Text {
                    text: qsTr("Volume")
                }

                Row {
                    id: volume_row_1
                    width: parent.width
                    spacing: 5

                    TextField {
                        id: volume_units_text_1
                        selectByMouse: true

                        onTextChanged: {
                            volume_output.text = converter_tool.convertVolume(
                                        volume_units_1.currentText,
                                        volume_units_2.currentText, text)
                        }
                    }

                    ComboBox {
                        id: volume_units_1
                        width: 200
                        model: converter_tool.volumeUnits

                        onCurrentTextChanged: {
                            volume_output.text = converter_tool.convertVolume(
                                        currentText,
                                        volume_units_2.currentText,
                                        volume_units_text_1.text)
                        }
                    }
                }

                Row {
                    id: volume_row_2
                    width: parent.width
                    spacing: 5

                    TextField {
                        id: volume_output
                        readOnly: true
                        selectByMouse: true
                    }

                    ComboBox {
                        id: volume_units_2
                        width: 200
                        model: converter_tool.volumeUnits

                        onCurrentTextChanged: {
                            volume_output.text = converter_tool.convertVolume(
                                        volume_units_1.currentText,
                                        currentText, volume_units_text_1.text)
                        }
                    }
                }

                Text {
                    text: qsTr("Weight")
                }

                Row {
                    id: weight_row_1
                    width: parent.width
                    spacing: 5

                    TextField {
                        id: weight_units_text_1
                        selectByMouse: true
                        onTextChanged: {
                            weight_output.text = converter_tool.convertWeight(
                                        weight_units_1.currentText,
                                        weight_units_2.currentText, text)
                        }
                    }

                    ComboBox {
                        id: weight_units_1
                        width: 200
                        model: converter_tool.weightUnits

                        onCurrentTextChanged: {
                            weight_output.text = converter_tool.convertWeight(
                                        currentText,
                                        weight_units_2.currentText,
                                        weight_units_text_1.text)
                        }
                    }
                }

                Row {
                    id: weight_row_2
                    width: parent.width
                    spacing: 5

                    TextField {
                        id: weight_output
                        readOnly: true
                        selectByMouse: true
                    }

                    ComboBox {
                        id: weight_units_2
                        width: 200
                        model: converter_tool.weightUnits

                        onCurrentTextChanged: {
                            weight_output.text = converter_tool.convertWeight(
                                        weight_units_1.currentText,
                                        currentText, weight_units_text_1.text)
                        }
                    }
                }

                Text {
                    text: qsTr("Money")
                }

                Row {
                    id: money_row_1
                    width: parent.width
                    spacing: 5

                    TextField {
                        id: money_units_text_1
                        selectByMouse: true

                        onTextChanged: {
                            money_output.text = converter_tool.convertMoney(
                                        money_units_1.currentText,
                                        money_units_2.currentText, text)
                        }
                    }

                    ComboBox {
                        id: money_units_1
                        width: 200
                        model: converter_tool.moneyUnits

                        onCurrentTextChanged: {
                            money_output.text = converter_tool.convertMoney(
                                        currentText, money_units_2.currentText,
                                        money_units_text_1.text)
                        }
                    }
                }

                Row {
                    id: money_row_2
                    width: parent.width
                    spacing: 5

                    TextField {
                        id: money_output
                        readOnly: true
                        selectByMouse: true
                    }

                    ComboBox {
                        id: money_units_2
                        width: 200
                        model: converter_tool.moneyUnits

                        onCurrentTextChanged: {
                            money_output.text = converter_tool.convertMoney(
                                        money_units_1.currentText, currentText,
                                        money_units_text_1.text)
                        }
                    }
                }
            }
        }
    }
}
