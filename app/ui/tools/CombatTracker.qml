import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import src
import ".."
import "./combat_tracker"

Page {
    id: root

    readonly property bool inPortrait: width < height || width < 1200
    property bool diceEnabled: true

    Component.onCompleted: CombatTrackerTool.loadData()

    // Top Bar
    header: ToolBar {
        id: top_bar

        Row {
            id: top_row
            anchors.fill: parent

            spacing: 10

            CustomToolBarButton {
                iconText: FontAwesome.chevronRight
                buttonText: qsTr("Next")
                onClicked: CombatTrackerTool.next()
                usesFixedWidth: false
            }

            CustomToolBarButton {
                iconText: FontAwesome.plus
                buttonText: qsTr("Add")
                usesFixedWidth: false
                onClicked: {
                    add_rect.visible = !add_rect.visible
                }
            }

            CustomToolBarButton {
                iconText: FontAwesome.dice
                buttonText: qsTr("Dice")
                onClicked: root.diceEnabled ? root.diceEnabled = false : root.diceEnabled = true
                usesFixedWidth: false
            }
        }
    }

    SplitView {
        id: split_view

        orientation: Qt.Horizontal
        anchors.fill: parent

        // Left Item
        Item {
            id: left_item
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            SplitView.minimumWidth: 500
            SplitView.fillWidth: true

            // List Header
            CharacterListHeader {
                id: list_header
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                height: top_bar.height
            }

            AddCharacterDialog {
                id: add_rect
                visible: false
                y: list_header.height
                width: list_header.width

                spinboxWidth: list_view.width / 6
            }

            // Combatant List
            CharacterListView {
                id: list_view

                anchors.left: parent.left
                anchors.top: list_header.bottom
                anchors.topMargin: add_rect.visible ? add_rect.height + 5 : 5
                anchors.bottom: parent.bottom
                anchors.right: parent.right

                Behavior on anchors.topMargin {
                    NumberAnimation {
                        duration: 200
                        easing.type: Easing.OutQuint
                    }
                }
            }
        }

        // Right Item
        Item {
            id: right_item
            visible: !root.inPortrait && root.diceEnabled

            anchors.top: parent.top
            anchors.bottom: parent.bottom

            SplitView.minimumWidth: 300
            SplitView.preferredWidth: 512

            Loader {
                id: combat_dice
                source: "dice/DiceCombatTracker.qml"
                anchors.fill: right_item
                asynchronous: true
            }

            Rectangle {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                color: palette.dark
                width: 1
            }
        }
    }

    footer: ToolBar {
        id: bottom_bar
        height: bottom_column.height

        Column {
            id: bottom_column
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            Loader {
                id: combat_dice_alt
                source: "DiceAlt.qml"

                anchors.left: parent.left
                anchors.right: parent.right

                visible: root.inPortrait && root.diceEnabled
                asynchronous: true
            }

            Item {
                id: bottom_rect
                height: Sizes.toolbarHeight
                anchors.left: parent.left
                anchors.right: parent.right

                Row {
                    id: bottom_row
                    height: parent.height
                    padding: 5
                    leftPadding: 0
                    spacing: 5

                    Row {
                        spacing: 10
                        padding: 10
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        width: 100

                        Label {
                            text: qsTr("Round:")
                            anchors.verticalCenter: parent.verticalCenter
                            font.bold: true
                            font.pointSize: 12
                        }

                        Label {
                            text: CombatTrackerTool.currentRound
                            anchors.verticalCenter: parent.verticalCenter
                            font.bold: true
                            font.pointSize: 12
                        }
                    }

                    CustomToolBarButton {
                        id: reset_button
                        iconText: FontAwesome.arrowRotateLeft
                        buttonText: qsTr("Reset")
                        usesFixedWidth: false
                        onClicked: CombatTrackerTool.reset()

                        toolTipText: qsTr("Start again at round 1 but keep all entries.")
                    }

                    CustomToolBarButton {
                        id: clear_button
                        iconText: FontAwesome.xmark
                        buttonText: qsTr("Clear")
                        usesFixedWidth: false
                        onClicked: CombatTrackerTool.clear()

                        toolTipText: qsTr("Remove all entries and reset counter to 1.")
                    }
                }
            }
        }
    }
}
