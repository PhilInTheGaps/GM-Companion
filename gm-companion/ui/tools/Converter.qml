import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3

import gm.companion.convertertool 1.0
import gm.companion.colorscheme 1.0
import gm.companion.platforms 1.0
import "../fontawesome"
import "./converter"

Page {
    id: converter

    ConverterTool {
        id: converter_tool
    }

    PlatformDetails {
        id: platform
    }

    ColorScheme {
        id: color_scheme
    }

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    Dialog {
        id: add_unit_dialog

        title: qsTr("Add Units")

        contentItem: Column {
            spacing: 5

            ComboBox {
                id: unit_type
                model: ["Length", "Area", "Volume", "Weight", "Money"]

                onCurrentTextChanged: refUnit_text.text = converter_tool.refUnitName(
                                          currentText)
            }

            Row {
                spacing: 5

                Grid {
                    spacing: 5
                    columns: 2

                    Text {
                        text: qsTr("Unit Name:")
                        color: color_scheme.textColor
                    }

                    Text {
                        id: refUnit_text
                        text: "Meters"
                        color: color_scheme.textColor
                    }

                    TextField {
                        id: unit_name
                    }

                    TextField {
                        id: unit_value
                    }
                }
            }

            Button {
                text: qsTr("Add Unit")

                onClicked: converter_tool.addUnit(unit_name.text,
                                                  unit_value.text,
                                                  unit_type.currentText)
            }
        }

        standardButtons: Dialog.Close
    }

    header: header_rect

    Rectangle {
        id: header_rect
        color: color_scheme.toolbarColor
        height: color_scheme.toolbarHeight

        Row {
            anchors.fill: parent
            padding: 5

            Button {
                hoverEnabled: true
                width: add_units_row.width
                height: parent.height - parent.padding * 2

                background: Rectangle {
                    color: "transparent"
                }

                Row {
                    id: add_units_row
                    anchors.centerIn: parent
                    spacing: 10
                    padding: 10

                    Icon {
                        icon: icons.fas_plus
                        pointSize: 15
                        anchors.verticalCenter: parent.verticalCenter
                        color: parent.parent.pressed ? "grey" : parent.parent.hovered ? "lightgrey" : "white"
                    }

                    Text {
                        id: add_units_text
                        text: qsTr("Add Custom Units")
                        anchors.verticalCenter: parent.verticalCenter
                        color: parent.parent.pressed ? "grey" : parent.parent.hovered ? "lightgrey" : color_scheme.toolbarTextColor
                        font.bold: true
                        font.pointSize: 12
                    }
                }

                onClicked: add_unit_dialog.open()
            }
        }
    }

    Rectangle {
        id: header_rect_2
        height: color_scheme.toolbarHeight * 2
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: side_bar.left

        color: color_scheme.backgroundColor

        Row {
            anchors.centerIn: parent
            padding: 10
            spacing: 10

            TextField {
                id: left_value_text_field
                width: 150
                height: header_rect_2.height * 2 / 3
                selectByMouse: true
                font.pointSize: 12

                onTextEdited: {
                    converter_tool.setUnit1ValueString(text)
                }
            }

            Rectangle {
                border.color: converter_tool.isUnit1Next ? "darkgreen" : color_scheme.primaryButtonColor
                border.width: converter_tool.isUnit1Next ? 3 : 1
                width: left_value_text_field.width
                height: parent.height - parent.padding * 2

                Text {
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

                    onClicked: {
                        converter_tool.setIsUnit1Next(true)
                    }
                }
            }

            Icon {
                icon: icons.fas_arrow_right
                pointSize: 15
                color: color_scheme.textColor
                anchors.verticalCenter: parent.verticalCenter
            }

            TextField {
                id: right_value_text_field
                width: 150
                height: header_rect_2.height * 2 / 3
                selectByMouse: true
                font.pointSize: 12
                readOnly: true
                text: converter_tool.unit2ValueString
            }

            Rectangle {
                border.color: !converter_tool.isUnit1Next ? "darkgreen" : color_scheme.primaryButtonColor
                border.width: !converter_tool.isUnit1Next ? 3 : 1
                width: right_value_text_field.width
                height: parent.height - parent.padding * 2

                Text {
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

                    onClicked: {
                        converter_tool.setIsUnit1Next(false)
                    }
                }
            }
        }
    }

    ConverterUnitPage {
        id: unit_page
        anchors.top: header_rect_2.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: side_bar.left

        categories: converter_tool.categories
        units: converter_tool.units

        onSetCategory: converter_tool.setCurrentCategory(category)
        onSetUnit: converter_tool.setUnit(unit)
    }

    Rectangle {
        id: side_bar
        color: color_scheme.toolbarColor
        width: color_scheme.toolbarHeight
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        Column {
            anchors.fill: parent

            Button {
                anchors.left: parent.left
                anchors.right: parent.right
                height: width
                hoverEnabled: true

                background: Rectangle {
                    color: "transparent"
                }

                Icon {
                    icon: icons.fas_ruler
                    pointSize: 15
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : color_scheme.toolbarTextColor
                    anchors.centerIn: parent
                }

                onClicked: converter_tool.setCurrentType(0)
            }

            Button {
                anchors.left: parent.left
                anchors.right: parent.right
                height: width
                hoverEnabled: true

                background: Rectangle {
                    color: "transparent"
                }

                Icon {
                    icon: icons.fas_ruler_combined
                    pointSize: 15
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : color_scheme.toolbarTextColor
                    anchors.centerIn: parent
                }

                onClicked: converter_tool.setCurrentType(1)
            }

            Button {
                anchors.left: parent.left
                anchors.right: parent.right
                height: width
                hoverEnabled: true

                background: Rectangle {
                    color: "transparent"
                }

                Icon {
                    icon: icons.fas_prescription_bottle
                    pointSize: 15
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : color_scheme.toolbarTextColor
                    anchors.centerIn: parent
                }

                onClicked: converter_tool.setCurrentType(2)
            }

            Button {
                anchors.left: parent.left
                anchors.right: parent.right
                height: width
                hoverEnabled: true

                background: Rectangle {
                    color: "transparent"
                }

                Icon {
                    icon: icons.fas_balance_scale
                    pointSize: 15
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : color_scheme.toolbarTextColor
                    anchors.centerIn: parent
                }

                onClicked: converter_tool.setCurrentType(3)
            }

            Button {
                anchors.left: parent.left
                anchors.right: parent.right
                height: width
                hoverEnabled: true

                background: Rectangle {
                    color: "transparent"
                }

                Icon {
                    icon: icons.fas_dollar_sign
                    pointSize: 15
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : color_scheme.toolbarTextColor
                    anchors.centerIn: parent
                }

                onClicked: converter_tool.setCurrentType(4)
            }
        }
    }
}
