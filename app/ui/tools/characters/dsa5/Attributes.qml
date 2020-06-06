import QtQuick 2.9
import QtQuick.Controls 2.2

import "../../../components"

Column {
    id: root
    topPadding: 20
    spacing: 30

    property var attribute_names: [qsTr("MU"), qsTr("KL"), qsTr(
            "IN"), qsTr("CH"), qsTr("FF"), qsTr("GE"), qsTr(
            "KO"), qsTr("KK"), qsTr("LE"), qsTr("AE"), qsTr(
            "KE"), qsTr("SK"), qsTr("ZK"), qsTr("AW"), qsTr(
            "INI"), qsTr("GS"), qsTr("WS")]
    property var colors: ["#9c3237", "#664a93", "#15a244", "#3f3f43", "#ac9e11", "#255175", "#f6f4ed", "#a1641e"]

    Row {
        spacing: 20
        anchors.horizontalCenter: parent.horizontalCenter

        Repeater {
            model: 8

            Column {
                spacing: 5

                Label {
                    text: root.attribute_names[index]
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: color_scheme.textColor
                    font.pointSize: 12
                }

                Rectangle {
                    border.width: 2
                    border.color: root.colors[index]
                    width: 60
                    height: 60
                    color: "transparent"

                    Label {
                        anchors.fill: parent
                        font.pixelSize: height - 30
                        font.bold: true

                        text: character_dsa5_viewer.attributes[index]
                        color: color_scheme.textColor
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }

    Row {
        spacing: 25
        anchors.horizontalCenter: parent.horizontalCenter

        Repeater {
            model: 9

            Column {
                spacing: 5

                Label {
                    text: root.attribute_names[index + 8]
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: color_scheme.textColor
                    font.pointSize: 12
                }

                Rectangle {
                    border.width: 1
                    border.color: "grey"
                    width: 50
                    height: 50
                    color: "transparent"

                    Label {
                        anchors.fill: parent
                        font.pixelSize: height - 30
                        font.bold: true

                        text: character_dsa5_viewer.attributes[index + 8]
                        color: color_scheme.textColor
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }
}
