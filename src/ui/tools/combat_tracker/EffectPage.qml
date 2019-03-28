import QtQuick 2.9
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0

Item {
    id: root

    readonly property bool hasTables: combat_tracker_effects.addons.length !== 0

    Component.onCompleted: combat_tracker_effects.loadAddons()

    TabBar {
        id: tab_bar
        height: color_scheme.toolbarHeight
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.leftMargin: 10

        background: Rectangle {
            color: color_scheme.toolbarColor
        }

        Repeater {
            id: addon_repeater
            model: combat_tracker_effects.addons

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

                onClicked: combat_tracker_effects.setCurrentAddon(modelData)

                background: Rectangle {
                    color: parent.pressed ? "black" : color_scheme.toolbarColor
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
            model: combat_tracker_effects.effectTypes

            Button {
                text: icon.source != "" ? "" : modelData
                onClicked: effect_text.text = combat_tracker_effects.randomEffect(
                               modelData)
                hoverEnabled: true

                ToolTip {
                    visible: parent.hovered && parent.text == ""
                    text: modelData
                }

                Image {
                    id: icon
                    source: combat_tracker_effects.getIcon(index)

                    anchors.fill: parent
                    anchors.margins: 5
                    sourceSize.height: height
                    sourceSize.width: width
                    smooth: true
                    asynchronous: true
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
        color: color_scheme.listHeaderBackgroundColor
        height: color_scheme.toolbarHeight * 2 + 20

        anchors.top: flow.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10

        Rectangle {
            anchors.fill: parent
            anchors.margins: 5

            color: color_scheme.backgroundColor

            ScrollView {
                anchors.fill: parent
                padding: 5
                clip: true
                contentHeight: effect_text.implicitHeight

                Text {
                    id: effect_text
                    font.pointSize: 12
                    width: parent.parent.width - 10
                    wrapMode: Text.WordWrap
                    color: color_scheme.textColor
                }
            }
        }
    }
}
