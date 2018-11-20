import QtQuick 2.9
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0
import gm.companion.colorscheme 1.0

import gm.companion.effecttool 1.0

Item {
    id: root

    readonly property bool hasTables: tool.addons.length !== 0

    Component.onCompleted: tool.loadAddons()

    EffectTool {
        id: tool
    }

    ColorScheme {
        id: colors
    }

    TabBar {
        id: tab_bar
        height: colors.toolbarHeight
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.leftMargin: 10

        background: Rectangle {
            color: colors.toolbarColor
        }

        Repeater {
            id: addon_repeater
            model: tool.addons

            TabButton {
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter

                Label {
                    text: modelData
                    color: "white"
                    anchors.centerIn: parent
                    font.pointSize: 12
                    font.bold: true
                }

                onClicked: tool.setCurrentAddon(modelData)

                background: Rectangle {
                    color: parent.pressed ? "black" : colors.toolbarColor
                }
            }
        }
    }

    Flow {
        id: flow
        anchors.top: tab_bar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10

        spacing: 5

        Repeater {
            id: type_repeater
            model: tool.effectTypes

            Button {
                text: icon.source != "" ? "" : modelData
                onClicked: effect_text.text = tool.randomEffect(modelData)
                hoverEnabled: true

                ToolTip {
                    visible: parent.hovered && parent.text == ""

                    text: modelData
                }

                Image {
                    id: icon
                    source: tool.getIcon(index)

                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.margins: 5
                    anchors.horizontalCenter: parent.horizontalCenter

                    sourceSize.height: height
                    sourceSize.width: width
                    smooth: true
                }

                ColorOverlay {
                    source: icon
                    color: "black"
                    anchors.fill: icon
                }

                Component.onCompleted: {
                    if (icon.source != "") {
                        width = height
                    }
                }
            }
        }
    }

    Rectangle {
        color: colors.listHeaderBackgroundColor
        height: colors.toolbarHeight * 2 + 20

        anchors.top: flow.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10

        Rectangle {
            anchors.fill: parent
            anchors.margins: 5

            color: colors.backgroundColor

            ScrollView {
                anchors.fill: parent
                padding: 5
                clip: true

                Text {
                    id: effect_text
                    font.pointSize: 12
                    width: parent.parent.width - 10
                    wrapMode: Text.WordWrap
                    color: colors.textColor
                }
            }
        }
    }
}
