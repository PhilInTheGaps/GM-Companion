pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import src
import ".."
import "./dice"
import "../common"

Page {
    id: dice_page

    property bool combat_tracker_mode: false
    property var diceSides: [4, 6, 8, 10, 12, 20]
    property alias diceTypeSpinBox: dice_type_spin_box
    readonly property int minimalHeight: main_column.height

    Connections {
        target: DiceTool

        function onCalculationStringChanged() {
            calculation_text_edit.text = DiceTool.calculationString
        }

        function onMixedCriticalResult() {
            roll_result.color = "orange"
            roll_result_help.visible = true
            roll_result_help.helpText = qsTr(
                        "Result contained both critical failures and successes")
        }

        function onSuccessfulCriticalResult() {
            roll_result.color = "green"
            roll_result_help.visible = true
            roll_result_help.helpText = qsTr(
                        "Result contained at least one critical success")
        }

        function onFailedCriticalResult() {
            roll_result.color = "red"
            roll_result_help.visible = true
            roll_result_help.helpText = qsTr(
                        "Result contained at least one critical failure")
        }

        function onNormalResult() {
            roll_result.color = dice_page.palette.text
            roll_result_help.visible = false
        }
    }

    DiceSettings {
        id: dice_settings_dialog
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
    }

    // Top Bar
    header: ToolBar {
        id: top_rect
        visible: !dice_page.combat_tracker_mode

        Row {
            id: top_row
            leftPadding: 10
            spacing: 10
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: dice_settings_button.left

            Repeater {
                model: dice_page.diceSides

                DiceButton {
                    required property int modelData

                    anchors.top: top_row.top
                    anchors.bottom: top_row.bottom
                    anchors.margins: 5

                    sides: modelData
                    isCurrentType: dice_type_spin_box.value == sides
                    onClicked: dice_type_spin_box.value = sides
                }
            }
        }

        CustomToolBarButton {
            id: dice_settings_button
            width: height
            anchors.right: parent.right
            anchors.margins: 10
            hoverEnabled: true

            iconText: FontAwesome.gear
            toolTipText: qsTr("Dice Settings")

            onClicked: dice_settings_dialog.open()
        }
    }

    // Main Column
    Column {
        id: main_column
        spacing: 10
        padding: 10
        topPadding: 0
        anchors.left: parent.left
        anchors.right: dice_page.combat_tracker_mode ? parent.right : undefined
        width: dice_page.combat_tracker_mode ? 0 : 620

        Row {
            id: spinbox_titles

            readonly property int textWidth: width / 3

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: parent.padding
            height: Sizes.toolbarHeight - 5

            Label {
                id: dice_count_text
                text: qsTr("Dice Count")
                font.pointSize: 16
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.bottom: parent.bottom
                width: parent.textWidth
            }

            Label {
                id: dice_sides_text
                text: qsTr("Dice Sides")
                font.pointSize: 16
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.bottom: parent.bottom
                width: parent.textWidth
            }

            Label {
                id: dice_modifier_text
                text: qsTr("Modifier")
                font.pointSize: 16
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.bottom: parent.bottom
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
                onValueChanged: DiceTool.setAmount(value)
                editable: true
                font.pixelSize: value_row.width / 25
            }

            SpinBox {
                id: dice_type_spin_box
                value: 20
                to: 1000
                width: (parent.width - parent.spacing * 2) / 3
                editable: true
                onValueChanged: DiceTool.setSides(value)
                font.pixelSize: value_row.width / 25
            }

            SpinBox {
                id: modifier_spin_box
                width: (parent.width - parent.spacing * 2) / 3
                value: 0
                from: -99
                to: 99
                onValueChanged: DiceTool.setModifier(value)
                editable: true
                font.pixelSize: value_row.width / 25
            }
        }

        Row {
            id: result_row
            anchors.horizontalCenter: parent.horizontalCenter
            width: value_row.width
            spacing: 10

            Button {
                contentItem: Label {
                    text: FontAwesome.dice
                    font.family: FontAwesome.fontSolid.family
                    font.styleName: FontAwesome.fontSolid.styleName
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 16
                }

                height: count_spinbox.height
                width: count_spinbox.width

                ToolTip.text: qsTr("Roll")
                ToolTip.visible: hovered
                hoverEnabled: true

                onClicked: {
                    roll_result.text = DiceTool.roll
                }
            }

            Label {
                text: qsTr("Result:")
                font.pixelSize: value_row.width / 20
                anchors.verticalCenter: parent.verticalCenter
                font.bold: true
            }

            Label {
                id: roll_result
                font.pixelSize: value_row.width / 20
                text: "-"
                anchors.verticalCenter: parent.verticalCenter
                font.bold: true
            }

            HelpAnnotation {
                id: roll_result_help
                visible: false
                anchors.verticalCenter: parent.verticalCenter
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
            color: palette.text
            selectedTextColor: palette.highlightedText
            selectionColor: palette.highlight

            font.pixelSize: value_row.width / 30
        }
    }
}
