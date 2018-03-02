import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4

import gm.companion.dicetool 1.0

Page {
    id: dice_page

    DiceTool {
        id: dice_tool

        onCalculationStringChanged: calculation_text_edit.text = calculationString
    }

    Row {
        anchors.fill: parent
        spacing: 10

        Column {
            id: dice_column
            height: parent.height - parent.padding
            spacing: 10
            padding: 5

            Text {
                text: qsTr("Default Dice")
            }

            Row {
                spacing: 5

                Button {
                    text: qsTr("D4")
                    height: width
                    onClicked: dice_type_spin_box.value = 4
                }

                Button {
                    text: qsTr("D6")
                    height: width
                    onClicked: dice_type_spin_box.value = 6
                }

                Button {
                    text: qsTr("D12")
                    height: width
                    onClicked: dice_type_spin_box.value = 12
                }

                Button {
                    text: qsTr("D20")
                    height: width
                    onClicked: dice_type_spin_box.value = 20
                }
            }

            Row {
                spacing: 10

                Column {
                    id: count_column

                    Text {
                        text: qsTr("Dice Count")
                    }

                    SpinBox {
                        id: count_spinbox
                        width: parent.width
                        height: 40
                        value: 1
                        onValueChanged: dice_tool.setAmount(value)
                    }
                }

                Column {
                    id: sides_column
                    Text {
                        text: qsTr("Dice Sides")
                    }

                    SpinBox {
                        id: dice_type_spin_box
                        value: 20
                        maximumValue: 1000
                        anchors.horizontalCenter: parent.horizontalCenter
                        height: 40
                        width: parent.width

                        onValueChanged: dice_tool.setSides(value)
                    }
                }

                Column {
                    Text {
                        text: qsTr("Modifier")
                    }

                    SpinBox {
                        width: parent.width
                        height: 40
                        value: 0
                        onValueChanged: dice_tool.setModifier(value)
                    }
                }

                //                Column {
                //                    Text {
                //                        text: qsTr("Bonus Dice")
                //                        ToolTip {
                //                            text: qsTr("Bonus Dice are ")
                //                        }
                //                    }
                //                }
            }

            Row {
                width: parent.width
                spacing: 5

                Button {
                    text: qsTr("Roll")
                    height: result_column.height

                    onClicked: {
                        roll_result.text = dice_tool.roll
                    }
                }

                Column {
                    id: result_column
                    Text {
                        text: qsTr("Result:")
                    }

                    Text {
                        id: roll_result
                        font.pixelSize: 35
                        text: "-"
                    }
                }
            }
        }

        ScrollView {
            height: parent.height - parent.padding
            width: parent.width - dice_column.width - parent.spacing - parent.padding

            flickableItem.interactive: true
            clip: true

            TextEdit {
                id: calculation_text_edit
                readOnly: true
            }
        }
    }
}
