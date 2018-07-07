import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4

import gm.companion.dicetool 1.0
import gm.companion.colorscheme 1.0
import gm.companion.platforms 1.0

Page {
    id: dice_page

    property bool combat_tracker_mode: false

    DiceTool {
        id: dice_tool

        onCalculationStringChanged: calculation_text_edit.text = calculationString

        onMixedCriticalResult: roll_result.color = "brown"
        onSuccessfulCriticalResult: roll_result.color = "green"
        onFailedCriticalResult: roll_result.color = "red"
        onNormalResult: roll_result.color = color_scheme.textColor
    }

    ColorScheme {
        id: color_scheme
    }

    PlatformDetails {
        id: platform
    }

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    Dialog {
        id: dice_settings_dialog
        title: qsTr("Dice Settings")

        contentItem: Column {
            padding: 5
            spacing: 5

            CheckBox {
                id: enable_criticals_checkbox
                text: qsTr("Enable Criticals")
                checked: dice_tool.getCriticalEnabled()
            }

            Text {
                text: qsTr("Critical Success") + ":"
            }

            SpinBox {
                id: success_spinbox
                from: 0
                to: 1000
                value: dice_tool.getSuccess()
                editable: true
            }

            Text {
                text: qsTr("Critical Failure") + ":"
            }

            SpinBox {
                id: failure_spinbox
                from: 0
                to: 1000
                value: dice_tool.getFailure()
                editable: true
            }
        }

        standardButtons: Dialog.Ok | Dialog.Cancel

        onAccepted: {
            dice_tool.setDiceSettings(enable_criticals_checkbox.checked,
                                      success_spinbox.value,
                                      failure_spinbox.value)
        }
    }

    Button {
        text: qsTr("Settings")
        x: 10
        y: 10
        visible: !combat_tracker_mode

        onClicked: dice_settings_dialog.open()
    }

    Column {
        id: dice_column
        spacing: 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        padding: combat_tracker_mode ? 10 : height * 0.15

        Row {
            id: dice_button_row
            spacing: 5
            anchors.horizontalCenter: parent.horizontalCenter

            Button {
                id: d4_button
                Image {
                    source: "/icons/dice/d4.png"
                    width: parent.height * 0.9
                    height: width

                    anchors.centerIn: parent
                    sourceSize.height: height
                    sourceSize.width: width
                }

                background: Rectangle {
                    color: parent.pressed ? "grey" : dice_type_spin_box.value
                                            == 4 ? "darkred" : "lightgrey"
                }

                ToolTip.text: qsTr("D4")
                ToolTip.visible: hovered
                hoverEnabled: true

                width: dice_page.width > 800 ? 800 / 6 : dice_page.width / 6
                height: width
                onClicked: dice_type_spin_box.value = 4
            }

            Button {
                Image {
                    source: "/icons/dice/d6.png"
                    width: parent.height * 0.9
                    height: width

                    anchors.centerIn: parent
                    sourceSize.height: height
                    sourceSize.width: width
                }

                ToolTip.text: qsTr("D6")
                ToolTip.visible: hovered
                hoverEnabled: true

                background: Rectangle {
                    color: parent.pressed ? "grey" : dice_type_spin_box.value
                                            == 6 ? "darkred" : "lightgrey"
                }

                width: dice_page.width > 800 ? 800 / 6 : dice_page.width / 6
                height: width
                onClicked: dice_type_spin_box.value = 6
            }

            Button {
                Image {
                    source: "/icons/dice/d12.png"
                    width: parent.height * 0.9
                    height: width

                    anchors.centerIn: parent
                    sourceSize.height: height
                    sourceSize.width: width
                }

                ToolTip.text: qsTr("D12")
                ToolTip.visible: hovered
                hoverEnabled: true

                background: Rectangle {
                    color: parent.pressed ? "grey" : dice_type_spin_box.value
                                            == 12 ? "darkred" : "lightgrey"
                }

                width: dice_page.width > 800 ? 800 / 6 : dice_page.width / 6
                height: width
                onClicked: dice_type_spin_box.value = 12
            }

            Button {
                Image {
                    source: "/icons/dice/d20.png"
                    width: parent.height * 0.9
                    height: width

                    anchors.centerIn: parent
                    sourceSize.height: height
                    sourceSize.width: width
                }

                ToolTip.text: qsTr("D20")
                ToolTip.visible: hovered
                hoverEnabled: true

                background: Rectangle {
                    color: parent.pressed ? "grey" : dice_type_spin_box.value
                                            == 20 ? "darkred" : "lightgrey"
                }

                width: dice_page.width > 800 ? 800 / 6 : dice_page.width / 6
                height: width
                onClicked: dice_type_spin_box.value = 20
            }
        }

        Row {
            id: value_row
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter

            Column {
                id: count_column

                Text {
                    id: dice_count_text
                    text: qsTr("Dice Count")
                    color: color_scheme.textColor
                    font.pixelSize: value_row.width / 25
                }

                SpinBox {
                    id: count_spinbox
                    height: dice_button_row.height / 2
                    width: dice_button_row.width / 3
                    value: 1
                    from: 1
                    onValueChanged: dice_tool.setAmount(value)
                    editable: true
                    font.pixelSize: value_row.width / 25
                }
            }

            Column {
                id: sides_column
                Text {
                    text: qsTr("Dice Sides")
                    color: color_scheme.textColor
                    font.pixelSize: value_row.width / 25
                }

                SpinBox {
                    id: dice_type_spin_box
                    value: 20
                    to: 1000
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: dice_button_row.height / 2
                    width: dice_button_row.width / 3
                    editable: true
                    onValueChanged: dice_tool.setSides(value)
                    font.pixelSize: value_row.width / 25
                }
            }

            Column {
                Text {
                    text: qsTr("Modifier")
                    color: color_scheme.textColor
                    font.pixelSize: value_row.width / 25
                }

                SpinBox {
                    id: modifier_spin_box
                    height: dice_button_row.height / 2
                    width: dice_button_row.width / 3
                    value: 0
                    from: -99
                    to: 99
                    onValueChanged: dice_tool.setModifier(value)
                    editable: true
                    font.pixelSize: value_row.width / 25
                }
            }
        }

        Row {
            id: result_row
            anchors.horizontalCenter: parent.horizontalCenter
            width: value_row.width
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

                height: d4_button.height / 2
                width: d4_button.width

                ToolTip.text: qsTr("Roll")
                ToolTip.visible: hovered
                hoverEnabled: true

                onClicked: {
                    roll_result.text = dice_tool.roll
                }
            }

            Text {
                text: qsTr("Result:")
                font.pixelSize: value_row.width / 25
                anchors.verticalCenter: parent.verticalCenter
                color: color_scheme.textColor
            }

            Text {
                id: roll_result
                font.pixelSize: value_row.width / 25
                text: "-"
                color: color_scheme.textColor
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        ScrollView {
            anchors.horizontalCenter: parent.horizontalCenter
            width: value_row.width

            height: parent.height - dice_button_row.height - value_row.height
                    - result_row.height - parent.spacing * 3 - parent.padding * 2
            clip: true

            TextEdit {
                id: calculation_text_edit
                readOnly: true
                color: color_scheme.textColor
                font.pixelSize: value_row.width / 40
            }
        }
    }
}
