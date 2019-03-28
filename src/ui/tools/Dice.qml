import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3

import FontAwesome 2.0

import "./dice"

Page {
    id: dice_page

    property bool combat_tracker_mode: false
    readonly property bool inPortrait: window.width < window.height
    property var diceSides: [4, 6, 8, 10, 12, 20]
    property alias diceTypeSpinBox: dice_type_spin_box
    readonly property int minimalHeight: main_column.height

    Connections {
        target: dice_tool

        onCalculationStringChanged: calculation_text_edit.text = dice_tool.calculationString

        onMixedCriticalResult: roll_result.color = "orange"
        onSuccessfulCriticalResult: roll_result.color = "green"
        onFailedCriticalResult: roll_result.color = "red"
        onNormalResult: roll_result.color = color_scheme.textColor
    }

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    header: top_rect

    DiceSettings {
        id: dice_settings_dialog
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
    }

    // Top Bar
    Rectangle {
        id: top_rect
        color: color_scheme.toolbarColor
        height: color_scheme.toolbarHeight
        visible: !combat_tracker_mode

        Row {
            id: top_row
            padding: 5
            leftPadding: 10
            spacing: 10
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: dice_settings_button.left

            Repeater {
                model: dice_page.diceSides

                DiceButton {
                    sides: modelData
                    isCurrentType: dice_type_spin_box.value == sides
                    onClicked: dice_type_spin_box.value = sides
                }
            }
        }

        Button {
            id: dice_settings_button
            width: height
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.margins: 10
            hoverEnabled: true

            background: Rectangle {
                color: "transparent"
            }

            Text {
                text: FontAwesome.cog
                font.family: FontAwesome.familySolid
                font.pointSize: 15
                anchors.centerIn: parent
                color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
            }

            onClicked: dice_settings_dialog.open()
        }
    }

    // Main Column
    Column {
        id: main_column
        spacing: 10
        padding: 10
        anchors.left: parent.left
        anchors.right: combat_tracker_mode
                       || inPortrait ? parent.right : undefined
        width: combat_tracker_mode || inPortrait ? 0 : 620

        Row {
            id: spinbox_titles

            readonly property int textWidth: width / 3

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: parent.padding

            Text {
                id: dice_count_text
                text: qsTr("Dice Count")
                color: color_scheme.textColor
                font.pixelSize: parent.width / 25
                horizontalAlignment: Text.AlignHCenter
                width: parent.textWidth
            }

            Text {
                id: dice_sides_text
                text: qsTr("Dice Sides")
                color: color_scheme.textColor
                font.pixelSize: parent.width / 25
                horizontalAlignment: Text.AlignHCenter
                width: parent.textWidth
            }

            Text {
                id: dice_modifier_text
                text: qsTr("Modifier")
                color: color_scheme.textColor
                font.pixelSize: parent.width / 25
                horizontalAlignment: Text.AlignHCenter
                width: parent.textWidth
            }
        }

        Row {
            id: value_row
            spacing: 10
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: parent.padding

            SpinBox {
                id: count_spinbox
                width: (parent.width - parent.spacing * 2) / 3
                value: 1
                from: 1
                onValueChanged: dice_tool.setAmount(value)
                editable: true
                font.pixelSize: value_row.width / 25
            }

            SpinBox {
                id: dice_type_spin_box
                value: 20
                to: 1000
                width: (parent.width - parent.spacing * 2) / 3
                editable: true
                onValueChanged: dice_tool.setSides(value)
                font.pixelSize: value_row.width / 25
            }

            SpinBox {
                id: modifier_spin_box
                width: (parent.width - parent.spacing * 2) / 3
                value: 0
                from: -99
                to: 99
                onValueChanged: dice_tool.setModifier(value)
                editable: true
                font.pixelSize: value_row.width / 25
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
    }

    ScrollView {
        anchors.top: main_column.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        clip: true
        padding: 10
        contentHeight: calculation_text_edit.implicitHeight

        TextEdit {
            id: calculation_text_edit
            readOnly: true
            color: color_scheme.textColor
            font.pixelSize: value_row.width / 30
        }
    }
}
