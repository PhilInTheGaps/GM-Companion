import QtQuick 2.9
import QtQuick.Controls 2.2
import "../../common"

Rectangle {
    id: list_header
    color: palette.highlight

    Grid {
        id: grid
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        columns: 5
        rows: 1
        verticalItemAlignment: Grid.AlignVCenter
        padding: 10
        spacing: 10

        Repeater {
            model: labels

            Item {
                width: parent.width / modelData.divisor
                height: 10

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.fill: modelData.alignment === "left" ? parent : undefined
                    spacing: help.visible > 0 ? 5 : 0

                    Item {
                        id: spacer
                        width: help.width
                        height: width
                        visible: help.visible
                    }

                    Label {
                        text: modelData.text
                        font.pointSize: 12
                        anchors.verticalCenter: parent.verticalCenter
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    HelpAnnotation {
                        id: help
                        helpText: modelData.helpText
                        visible: helpText.length > 0
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
        }
    }

    readonly property var labels: [{
            "text": qsTr("Name"),
            "divisor": 5,
            "alignment": "left",
            "helpText": ""
        }, {
            "text": qsTr("Initiative"),
            "divisor": 6,
            "alignment": "center",
            "helpText": ""
        }, {
            "text": qsTr("Health"),
            "divisor": 6,
            "alignment": "center",
            "helpText": ""
        }, {
            "text": qsTr("Priority"),
            "divisor": 6,
            "alignment": "center",
            "helpText": qsTr("Characters with the same initiative value will be ordered by their priority.")
        }, {
            "text": qsTr("Notes"),
            "divisor": 6,
            "alignment": "left",
            "helpText": ""
        }]
}
