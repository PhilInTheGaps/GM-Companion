import QtQuick 2.9
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0
import "../../defines.js" as Defines

Item {
    id: root

    readonly property bool hasTables: combat_tracker_effects.addons.length !== 0

    Component.onCompleted: combat_tracker_effects.loadAddons()

    TabBar {
        id: tab_bar
        height: Defines.TOOLBAR_HEIGHT
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.leftMargin: 10

        background: Rectangle {
            color: palette.alternateBase
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
                    color: parent.pressed ? palette.dark : palette.alternateBase
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
        color: palette.button
        height: Defines.TOOLBAR_HEIGHT * 2 + 20

        anchors.top: flow.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10

        Rectangle {
            anchors.fill: parent
            anchors.margins: 5

            color: palette.base

            ScrollView {
                anchors.fill: parent
                padding: 5
                clip: true
                contentHeight: effect_text.implicitHeight

                Label {
                    id: effect_text
                    font.pointSize: 12
                    width: parent.parent.width - 10
                    wrapMode: Text.WordWrap
                }
            }
        }
    }
}
