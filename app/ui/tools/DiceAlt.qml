import QtQuick 2.9
import QtQuick.Controls 2.2
import FontAwesome 2.0
import "../common"

Item {
    id: dice_page
    height: main_item.height

    Connections {
        target: dice_tool

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
            roll_result.color = palette.text
            roll_result_help.visible = false
        }
    }

    Rectangle {
        id: main_item
        height: dice_row.height
        anchors.left: parent.left
        anchors.right: parent.right
        color: palette.alternateBase

        Row {
            id: dice_row
            spacing: 10
            padding: 10
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter

            // Dice Count
            SpinBox {
                id: count_spinbox
                value: 1
                from: 1
                onValueChanged: dice_tool.setAmount(value)
                editable: true
                font.pixelSize: parent.height * 0.4
            }

            Label {
                text: qsTr("D")
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: parent.height * 0.5
                font.bold: true
            }

            // Dice Type
            SpinBox {
                id: dice_type_spin_box
                value: 20
                to: 1000
                editable: true
                onValueChanged: dice_tool.setSides(value)
                font.pixelSize: parent.height * 0.4
            }

            Label {
                text: "+"
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: parent.height * 0.5
                font.bold: true
            }

            // Modifier
            SpinBox {
                value: 0
                from: -99
                to: 99
                onValueChanged: dice_tool.setModifier(value)
                editable: true
                font.pixelSize: parent.height * 0.4
            }

            // "Roll!"
            Button {
                height: dice_type_spin_box.height

                contentItem: Label {
                    text: FontAwesome.dice
                    font.family: FontAwesome.familySolid
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 16
                }

                ToolTip.text: qsTr("Roll")
                ToolTip.visible: hovered
                hoverEnabled: true

                onClicked: {
                    roll_result.text = dice_tool.roll
                }
            }

            Label {
                text: qsTr(":")
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: parent.height * 0.5
                font.bold: true
            }

            // Result
            Label {
                id: roll_result
                text: "-"
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: parent.height * 0.5
                font.bold: true
            }

            HelpAnnotation {
                id: roll_result_help
                visible: false
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }
}
