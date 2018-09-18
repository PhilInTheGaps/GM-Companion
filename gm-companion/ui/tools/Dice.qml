import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4

import gm.companion.dicetool 1.0
import gm.companion.colorscheme 1.0
import gm.companion.platforms 1.0
import "../fontawesome"

Page {
    id: dice_page

    property bool combat_tracker_mode: false
    readonly property bool inPortrait: window.width < window.height

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

    header: top_rect

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

            RadioButton {
                id: min_max_radiobutton
                text: qsTr("Min / Max")
                checked: dice_tool.getMinMax()
            }

            Column {
                padding: 10
                spacing: 5

                RadioButton {
                    id: success_max_radiobutton
                    text: qsTr("Success: Maximum, Failure: Minimum")
                    checked: dice_tool.getSuccessMax()
                }

                RadioButton {
                    text: qsTr("Success: Minimum, Failure: Maximum")
                    checked: !dice_tool.getSuccessMax()
                }
            }

            RadioButton {
                text: qsTr("Fixed Values")
                checked: !dice_tool.getMinMax()
            }

            Column {
                padding: 10
                spacing: 5

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
        }

        standardButtons: Dialog.Ok | Dialog.Cancel

        onAccepted: {
            dice_tool.setDiceSettings(enable_criticals_checkbox.checked,
                                      success_spinbox.value,
                                      failure_spinbox.value,
                                      min_max_radiobutton.checked,
                                      success_max_radiobutton.checked)
        }
    }

    Rectangle {
        id: top_rect
        color: color_scheme.toolbarColor
        height: color_scheme.toolbarHeight
        visible: !combat_tracker_mode

        Row {
            id: top_row
            padding: 10
            spacing: 10
            anchors.fill: parent

            Button {
                width: settings_row.width
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                hoverEnabled: true

                background: Rectangle {
                    color: "transparent"
                }

                Row {
                    id: settings_row
                    padding: 10
                    spacing: 10
                    anchors.centerIn: parent

                    Icon {
                        icon: icons.fas_cog
                        pointSize: 15
                        anchors.verticalCenter: parent.verticalCenter
                        color: parent.parent.pressed ? "grey" : parent.parent.hovered ? "lightgrey" : "white"
                    }

                    Text {
                        text: qsTr("Settings")
                        font.pointSize: 12
                        font.bold: true
                        anchors.verticalCenter: parent.verticalCenter
                        color: parent.parent.pressed ? "grey" : parent.parent.hovered ? "lightgrey" : "white"
                    }
                }

                onClicked: dice_settings_dialog.open()
            }
        }
    }

    Row {
        id: dice_row
        spacing: 10
        padding: 10
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        Column {
            id: dice_button_column
            spacing: 10

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

                width: combat_tracker_mode
                       || inPortrait ? dice_page.width / 8 : 100
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

                width: combat_tracker_mode
                       || inPortrait ? dice_page.width / 8 : 100
                height: width
                onClicked: dice_type_spin_box.value = 6
            }

            Button {
                id: d8_button
                Image {
                    source: "/icons/dice/d8.png"
                    width: parent.height * 0.9
                    height: width

                    anchors.centerIn: parent
                    sourceSize.height: height
                    sourceSize.width: width
                }

                background: Rectangle {
                    color: parent.pressed ? "grey" : dice_type_spin_box.value
                                            == 8 ? "darkred" : "lightgrey"
                }

                ToolTip.text: qsTr("D8")
                ToolTip.visible: hovered
                hoverEnabled: true

                width: combat_tracker_mode
                       || inPortrait ? dice_page.width / 8 : 100
                height: width
                onClicked: dice_type_spin_box.value = 8
            }

            Button {
                id: d10_button
                Image {
                    source: "/icons/dice/d10.png"
                    width: parent.height * 0.9
                    height: width

                    anchors.centerIn: parent
                    sourceSize.height: height
                    sourceSize.width: width
                }

                background: Rectangle {
                    color: parent.pressed ? "grey" : dice_type_spin_box.value
                                            == 10 ? "darkred" : "lightgrey"
                }

                ToolTip.text: qsTr("D10")
                ToolTip.visible: hovered
                hoverEnabled: true

                width: combat_tracker_mode
                       || inPortrait ? dice_page.width / 8 : 100
                height: width
                onClicked: dice_type_spin_box.value = 10
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

                width: combat_tracker_mode
                       || inPortrait ? dice_page.width / 8 : 100
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

                width: combat_tracker_mode
                       || inPortrait ? dice_page.width / 8 : 100
                height: width
                onClicked: dice_type_spin_box.value = 20
            }
        }

        Column {
            id: main_column
            height: parent.height - parent.padding * 2
            spacing: 10

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
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    SpinBox {
                        id: count_spinbox
                        height: d4_button.height - dice_count_text.height
                        width: combat_tracker_mode
                               || inPortrait ? (dice_page.width - dice_button_column.width
                                                - dice_row.padding * 2 - dice_row.spacing
                                                - main_column.spacing * 2) / 3 : 200
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
                        id: dice_sides_text
                        text: qsTr("Dice Sides")
                        color: color_scheme.textColor
                        font.pixelSize: value_row.width / 25
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    SpinBox {
                        id: dice_type_spin_box
                        value: 20
                        to: 1000
                        anchors.horizontalCenter: parent.horizontalCenter
                        height: d4_button.height - dice_sides_text.height
                        width: combat_tracker_mode
                               || inPortrait ? (dice_page.width - dice_button_column.width
                                                - dice_row.padding * 2 - dice_row.spacing
                                                - main_column.spacing * 2) / 3 : 200
                        editable: true
                        onValueChanged: dice_tool.setSides(value)
                        font.pixelSize: value_row.width / 25
                    }
                }

                Column {
                    Text {
                        id: dice_modifier_text
                        text: qsTr("Modifier")
                        color: color_scheme.textColor
                        font.pixelSize: value_row.width / 25
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    SpinBox {
                        id: modifier_spin_box
                        height: d4_button.height - dice_modifier_text.height
                        width: combat_tracker_mode
                               || inPortrait ? (dice_page.width - dice_button_column.width
                                                - dice_row.padding * 2 - dice_row.spacing
                                                - main_column.spacing * 2) / 3 : 200
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

                    height: count_spinbox.height
                    width: count_spinbox.width

                    ToolTip.text: qsTr("Roll")
                    ToolTip.visible: hovered
                    hoverEnabled: true

                    onClicked: {
                        roll_result.text = dice_tool.roll
                    }
                }

                Text {
                    text: qsTr("Result:")
                    font.pixelSize: value_row.width / 20
                    anchors.verticalCenter: parent.verticalCenter
                    color: color_scheme.textColor
                    font.bold: true
                }

                Text {
                    id: roll_result
                    font.pixelSize: value_row.width / 20
                    text: "-"
                    color: color_scheme.textColor
                    anchors.verticalCenter: parent.verticalCenter
                    font.bold: true
                }
            }

            ScrollView {
                anchors.horizontalCenter: parent.horizontalCenter
                width: value_row.width

                height: parent.height - parent.padding - y
                clip: true

                TextEdit {
                    id: calculation_text_edit
                    readOnly: true
                    color: color_scheme.textColor
                    font.pixelSize: value_row.width / 30
                }
            }
        }
    }
}
