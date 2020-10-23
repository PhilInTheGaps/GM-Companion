import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../sizes.js" as Sizes
import "./dice"

Page {
    id: dice_page

    property bool combat_tracker_mode: false
    readonly property bool inPortrait: width < height
    property var diceSides: [4, 6, 8, 10, 12, 20]
    property alias diceTypeSpinBox: dice_type_spin_box
    readonly property int minimalHeight: main_column.height

    Connections {
        target: dice_tool

        function onCalculationStringChanged() {
            calculation_text_edit.text = dice_tool.calculationString
        }

        function onMixedCriticalResult() {
            roll_result.color = "orange"
        }

        function onSuccessfulCriticalResult() {
            roll_result.color = "green"
        }

        function onFailedCriticalResult() {
            roll_result.color = "red"
        }

        function onNormalResult() {
            roll_result.color = palette.text
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

        CustomToolBarButton {
            id: dice_settings_button
            width: height
            anchors.right: parent.right
            anchors.margins: 10
            hoverEnabled: true

            iconText: FontAwesome.cog
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
        anchors.right: combat_tracker_mode
                       || inPortrait ? parent.right : undefined
        width: combat_tracker_mode || inPortrait ? 0 : 620

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
            spacing: 10

            Button {
                contentItem: Label {
                    text: FontAwesome.dice
                    font.family: FontAwesome.familySolid
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
                    roll_result.text = dice_tool.roll
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
