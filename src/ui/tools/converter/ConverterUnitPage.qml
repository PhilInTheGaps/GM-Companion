import QtQuick 2.9
import QtQuick.Controls 2.2
import FontAwesome 2.0

Page {
    id: page

    signal setCategory(int category)
    signal setUnit(int unit)
    signal deleteUnit(int unit)

    property var categories
    property var units
    property string category: ""

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    footer: footer_rect

    ScrollView {
        id: main_scroll
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: 500
        clip: true
        bottomPadding: 10
        contentHeight: main_column.implicitHeight

        Column {
            id: main_column
            spacing: 10
            width: 500
            padding: 10

            Repeater {
                id: unit_repeater
                model: units

                Button {
                    text: modelData
                    width: main_column.width - main_column.padding * 2
                    hoverEnabled: true

                    font.pointSize: 12

                    Button {
                        width: times_icon.width + 20
                        hoverEnabled: true

                        Text {
                            id: times_icon
                            text: FontAwesome.times
                            font.family: FontAwesome.familySolid
                            font.pointSize: 15
                            anchors.centerIn: parent
                            color: parent.pressed ? "darkgrey" : parent.hovered ? "grey" : "black"
                            visible: parent.parent.hovered
                                     && category == "Custom"
                        }

                        background: Rectangle {
                            color: "transparent"
                        }

                        x: parent.width - width
                        anchors.verticalCenter: parent.verticalCenter

                        onClicked: deleteUnit(index)
                    }

                    onClicked: setUnit(index)
                }
            }
        }
    }

    Rectangle {
        id: footer_rect
        height: category_flow.height
        color: color_scheme.toolbarColor

        Flow {
            id: category_flow
            spacing: 10
            leftPadding: 5
            rightPadding: 5
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            Repeater {
                id: category_repeater
                model: categories

                onModelChanged: category = categories[0]

                Button {
                    height: color_scheme.toolbarHeight
                    width: category_text.width
                    hoverEnabled: true

                    background: Rectangle {
                        color: "transparent"
                    }

                    Text {
                        id: category_text
                        text: modelData
                        font.bold: true
                        font.pointSize: 13
                        color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : color_scheme.toolbarTextColor
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    onClicked: {
                        setCategory(index)
                        category = categories[index]
                    }
                }
            }
        }
    }
}
