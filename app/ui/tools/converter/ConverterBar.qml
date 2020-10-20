import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../../defines.js" as Defines

Item {
    id: root
    height: Defines.TOOLBAR_HEIGHT * 2

    Row {
        anchors.centerIn: parent
        padding: 10
        spacing: 10

        TextField {
            id: left_value_text_field
            width: 150
            height: root.height * 2 / 3
            font.pointSize: 12
            text: "1.0"

            onTextEdited: converter_tool.setUnit1ValueString(text)
        }

        Rectangle {
            border.color: converter_tool.isUnit1Next ? "darkgreen" : palette.button
            border.width: converter_tool.isUnit1Next ? 3 : 1
            color: "transparent"
            width: left_value_text_field.width
            height: parent.height - parent.padding * 2

            Label {
                id: left_unit_text
                font.pointSize: 14
                text: converter_tool.unit1
                width: parent.width - 10
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignHCenter
                anchors.centerIn: parent
            }

            MouseArea {
                anchors.fill: parent
                onClicked: converter_tool.setIsUnit1Next(true)
            }
        }

        Label {
            text: FontAwesome.arrowRight
            font.family: FontAwesome.familySolid
            font.pointSize: 15
            anchors.verticalCenter: parent.verticalCenter
        }

        TextField {
            id: right_value_text_field
            width: 150
            height: root.height * 2 / 3
            font.pointSize: 12
            readOnly: true
            text: converter_tool.unit2ValueString
        }

        Rectangle {
            border.color: !converter_tool.isUnit1Next ? "darkgreen" : palette.button
            border.width: !converter_tool.isUnit1Next ? 3 : 1
            color: "transparent"
            width: right_value_text_field.width
            height: parent.height - parent.padding * 2

            Label {
                id: right_unit_text
                font.pointSize: 14
                text: converter_tool.unit2
                anchors.centerIn: parent
                width: parent.width - 10
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignHCenter
            }

            MouseArea {
                anchors.fill: parent
                onClicked: converter_tool.setIsUnit1Next(false)
            }
        }
    }
}
