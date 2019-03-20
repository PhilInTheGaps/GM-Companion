import QtQuick 2.9
import QtQuick.Controls 2.2

Item {
    Row {
        spacing: 50
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 20

        Column {
            spacing: 10

            Repeater {
                model: character_dsa5_viewer.advantages

                Label {
                    text: modelData
                    color: color_scheme.textColor
                    font.pointSize: 12
                }
            }
        }

        Column {
            spacing: 10

            Repeater {
                model: character_dsa5_viewer.disadvantages

                Label {
                    text: modelData
                    color: color_scheme.textColor
                    font.pointSize: 12
                }
            }
        }
    }
}
