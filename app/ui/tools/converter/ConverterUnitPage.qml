import QtQuick 2.9
import QtQuick.Controls 2.2
import FontAwesome 2.0
import CustomComponents 1.0
import "../../defines.js" as Defines

Page {
    id: page

    Component.onCompleted: {
        from_value_text_field.forceActiveFocus()
    }

    Item {
        id: split_view
        anchors.fill: parent
        anchors.margins: 10

        Item {
            id: left_view
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: center_view.left
            anchors.rightMargin: 5

            TextField {
                id: from_value_text_field

                anchors.top: parent.top
                anchors.right: parent.right

                width: Math.min(Defines.SIDEBAR_WIDTH * 2, left_view.width)
                height: Defines.TOOLBAR_HEIGHT * 1.5
                font.pointSize: 12
                text: qsTr("1.0")

                validator: DoubleValidator {
                    notation: DoubleValidator.ScientificNotation
                }

                selectByMouse: true
            }

            Flickable {
                id: left_scroll

                anchors.top: from_value_text_field.bottom
                anchors.topMargin: 10
                anchors.bottom: parent.bottom
                anchors.right: parent.right

                width: from_value_text_field.width

                clip: true
                contentHeight: left_column.implicitHeight
                interactive: left_scroll_bar.visible

                ScrollBar.vertical: ScrollBar {
                    id: left_scroll_bar
                    anchors.right: parent.right
                    visible: left_scroll.contentHeight > left_scroll.height
                }

                Column {
                    id: left_column
                    spacing: 10

                    width: left_scroll_bar.visible ? parent.width - left_scroll_bar.width
                                                     - 5 : parent.width

                    Repeater {
                        id: from_unit_repeater

                        model: converter_tool
                               && converter_tool.currentCategory ? converter_tool.currentCategory.units : []

                        CustomButton {
                            buttonText: qsTranslate("Units", modelData.name)
                            anchors.left: parent.left
                            anchors.right: parent.right
                            hoverEnabled: true
                            font.pointSize: 12

                            backgroundColor: converter_tool
                                             && converter_tool.fromUnit
                                             === modelData ? palette.button : palette.dark

                            onClicked: {
                                if (!converter_tool)
                                    return

                                converter_tool.fromUnit = modelData
                            }
                        }
                    }
                }
            }
        }

        Item {
            id: center_view
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: arrow_label.width

            Label {
                id: arrow_label
                text: FontAwesome.arrowRight
                font.family: FontAwesome.familySolid
                font.pointSize: 15

                height: Defines.TOOLBAR_HEIGHT * 1.5
                verticalAlignment: Label.AlignVCenter

                anchors.top: parent.top
            }
        }

        Item {
            id: right_view
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.left: center_view.right
            anchors.leftMargin: 5

            TextField {
                id: to_value_text_field

                anchors.top: parent.top
                anchors.left: parent.left

                width: Math.min(Defines.SIDEBAR_WIDTH * 2, right_view.width)
                height: Defines.TOOLBAR_HEIGHT * 1.5
                font.pointSize: 12
                readOnly: true
                text: converter_tool ? converter_tool.convert(
                                           converter_tool.fromUnit,
                                           from_value_text_field.text,
                                           converter_tool.toUnit) : ""
            }

            Flickable {
                id: right_scroll

                anchors.top: to_value_text_field.bottom
                anchors.topMargin: 10
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                width: to_value_text_field.width

                clip: true
                contentHeight: right_column.implicitHeight
                interactive: right_scroll_bar.visible

                ScrollBar.vertical: ScrollBar {
                    id: right_scroll_bar
                    anchors.right: parent.right
                    visible: right_scroll.contentHeight > right_scroll.height
                }

                Column {
                    id: right_column
                    spacing: 10

                    width: right_scroll_bar.visible ? parent.width - right_scroll_bar.width
                                                      - 5 : parent.width

                    Repeater {
                        id: to_unit_repeater

                        model: converter_tool
                               && converter_tool.currentCategory ? converter_tool.currentCategory.units : []

                        CustomButton {
                            property string converted: converter_tool ? converter_tool.convert(
                                                                            converter_tool.fromUnit, from_value_text_field.text,
                                                                            modelData) : ""

                            buttonText: qsTranslate("Units", modelData.name)
                                        + (converted.length > 0 ? " (" + converted + ")" : "")
                            anchors.left: parent.left
                            anchors.right: parent.right
                            hoverEnabled: true
                            font.pointSize: 12

                            backgroundColor: converter_tool
                                             && converter_tool.toUnit
                                             === modelData ? palette.button : palette.dark

                            onClicked: {
                                if (!converter_tool)
                                    return

                                converter_tool.toUnit = modelData
                            }
                        }
                    }
                }
            }
        }
    }
}
