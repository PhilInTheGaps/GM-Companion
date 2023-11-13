import QtQuick
import QtQuick.Controls
import IconFonts
import src
import common
import "../common"

Item {
    id: dice_page
    height: main_item.height

    Connections {
        target: DiceTool

        function onMixedCriticalResult() {
            roll_result.color = SettingsManager.colors.warning;
            roll_result_help.visible = true;
            roll_result_help.helpText = qsTr("Result contained both critical failures and successes");
        }

        function onSuccessfulCriticalResult() {
            roll_result.color = SettingsManager.colors.success;
            roll_result_help.visible = true;
            roll_result_help.helpText = qsTr("Result contained at least one critical success");
        }

        function onFailedCriticalResult() {
            roll_result.color = SettingsManager.colors.error;
            roll_result_help.visible = true;
            roll_result_help.helpText = qsTr("Result contained at least one critical failure");
        }

        function onNormalResult() {
            roll_result.color = dice_page.palette.text;
            roll_result_help.visible = false;
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
                to: 9999
                onValueChanged: DiceTool.setAmount(value)
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
                to: 9999
                editable: true
                onValueChanged: DiceTool.sides = value
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
                from: -9999
                to: 9999
                onValueChanged: DiceTool.setModifier(value)
                editable: true
                font.pixelSize: parent.height * 0.4
            }

            // "Roll!"
            Button {
                height: dice_type_spin_box.height

                contentItem: Label {
                    text: FontAwesome.dice
                    font.family: FontAwesome.fontSolid.family
                    font.styleName: FontAwesome.fontSolid.styleName
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 16
                }

                ToolTip.text: qsTr("Roll")
                ToolTip.visible: hovered
                hoverEnabled: true

                onClicked: {
                    DiceTool.roll();
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
                text: DiceTool.result.length > 0 ? DiceTool.result : "-"
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
