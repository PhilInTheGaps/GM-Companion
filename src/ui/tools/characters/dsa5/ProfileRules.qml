import QtQuick 2.9
import QtQuick.Controls 2.2

import "../../../components"

ScrollView {
    clip: true
    contentHeight: main_column.implicitHeight

    Column {
        id: main_column
        anchors.left: parent.left
        anchors.right: parent.right
        topPadding: 5
        spacing: 20

        Column {
            spacing: 10
            Label {
                text: qsTr("Regelbasis")
                font.bold: true
                color: color_scheme.textColor
                font.pointSize: 12
            }

            Repeater {
                model: character_dsa5_viewer.ruleBooks

                Label {
                    text: modelData
                    color: color_scheme.textColor
                }
            }
        }

        Column {
            spacing: 10
            Label {
                text: qsTr("Optionalregeln")
                font.bold: true
                color: color_scheme.textColor
                font.pointSize: 12
            }

            Repeater {
                model: character_dsa5_viewer.optionalRules

                Label {
                    text: modelData
                    color: color_scheme.textColor
                }
            }
        }
    }
}
