import QtQuick 2.9
import QtQuick.Controls 2.2

Page {
    id: page

    signal setCategory(int category)
    signal setUnit(int unit)

    property var categories
    property var units

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
                    width: parent.width

                    font.pointSize: 12

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
                    }
                }
            }
        }
    }
}
