import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4

import gm.companion.dicetool 1.0

Page {
    id: dice_page

    DiceTool {
        id: dice_tool

        onCalculationStringChanged: calculation_text_edit.text = calculationString
    }

    Flow {
        anchors.fill: parent
        spacing: 10

        Column {
            id: dice_column
            spacing: 10

            Row {
                id: dice_button_row
                spacing: 5

                Button {

                    Image {
                        source: "/icons/dice/d4.png"
                        width: parent.height * 0.9
                        height: width

                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter

                        sourceSize.height: height
                        sourceSize.width: width
                    }

                    ToolTip.text: qsTr("D4")
                    ToolTip.visible: hovered
                    hoverEnabled: true

                    width: (value_row.width - 3 * parent.spacing) / 4
                    height: width
                    onClicked: dice_type_spin_box.value = 4
                }

                Button {
                    Image {
                        source: "/icons/dice/d6.png"
                        width: parent.height * 0.9
                        height: width

                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter

                        sourceSize.height: height
                        sourceSize.width: width
                    }

                    ToolTip.text: qsTr("D6")
                    ToolTip.visible: hovered
                    hoverEnabled: true

                    width: (value_row.width - 3 * parent.spacing) / 4
                    height: width
                    onClicked: dice_type_spin_box.value = 6
                }

                Button {
                    Image {
                        source: "/icons/dice/d12.png"
                        width: parent.height * 0.9
                        height: width

                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter

                        sourceSize.height: height
                        sourceSize.width: width
                    }

                    ToolTip.text: qsTr("D12")
                    ToolTip.visible: hovered
                    hoverEnabled: true

                    width: (value_row.width - 3 * parent.spacing) / 4
                    height: width
                    onClicked: dice_type_spin_box.value = 12
                }

                Button {
                    Image {
                        source: "/icons/dice/d20.png"
                        width: parent.height * 0.9
                        height: width

                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter

                        sourceSize.height: height
                        sourceSize.width: width
                    }

                    ToolTip.text: qsTr("D20")
                    ToolTip.visible: hovered
                    hoverEnabled: true

                    width: (value_row.width - 3 * parent.spacing) / 4
                    height: width
                    onClicked: dice_type_spin_box.value = 20
                }
            }

            Row {
                id: value_row
                spacing: 10

                Column {
                    id: count_column

                    Text {
                        id: dice_count_text
                        text: qsTr("Dice Count")
                    }

                    SpinBox {
                        id: count_spinbox
                        height: 40
                        value: 1
                        from: 1
                        onValueChanged: dice_tool.setAmount(value)
                        editable: true
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
                        to: 1000
                        anchors.horizontalCenter: parent.horizontalCenter
                        height: 40
                        editable: true
                        onValueChanged: dice_tool.setSides(value)
                    }
                }

                Column {
                    Text {
                        text: qsTr("Modifier")
                    }

                    SpinBox {

                        height: 40
                        value: 0
                        from: -99
                        to: 99
                        onValueChanged: dice_tool.setModifier(value)
                        editable: true
                    }
                }
            }

            Row {
                width: parent.width
                spacing: 5

                Button {
                    Image {
                        source: "/icons/dice/dice_roll.png"
                        width: parent.height * 0.9
                        height: width

                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter

                        sourceSize.height: height
                        sourceSize.width: width
                    }

                    height: roll_result.height

                    ToolTip.text: qsTr("Roll")
                    ToolTip.visible: hovered
                    hoverEnabled: true

                    onClicked: {
                        roll_result.text = dice_tool.roll
                    }
                }

                Text {
                    text: qsTr("Result:")
                    font.pixelSize: 35
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    id: roll_result
                    font.pixelSize: 35
                    text: "-"
                }
            }
        }

        ScrollView {
            height: parent.width > dice_column.width
                    * 2 ? parent.height : parent.height - dice_column.height - parent.spacing

            width: parent.width > dice_column.width
                   * 2 ? parent.width - parent.spacing - dice_column.width : parent.width

            clip: true

            TextEdit {
                id: calculation_text_edit
                readOnly: true
            }
        }
    }
}
