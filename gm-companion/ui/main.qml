import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import "./tools"
import "./menu"
import gm.companion.platforms 1.0

Window {
    id: window
    visible: true
    width: 1280
    height: 720
    title: qsTr("GM-Companion")

    //! [orientation]
    readonly property bool inPortrait: window.width < window.height

    PlatformDetails {
        id: platform
    }

    //! [orientation]
    ToolBar {
        id: toolbar
        width: {
            (inPortrait) ? parent.width : parent.width - drawer.width
        }
        height: 40
        anchors.right: parent.right

        Row {
            id: toolbar_row_left
            height: parent.height

            ToolButton {
                id: tools_button
                text: "Menu"
                height: parent.height
                visible: (inPortrait) ? true : false

                onClicked: drawer.open()
            }
        }

        Label {
            id: tool_label
            text: "Current Tool: Audio"
            height: parent.height
            verticalAlignment: "AlignVCenter"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: toolbar_row_left.verticalCenter
        }
    }

    Rectangle {
        id: drawer_bg
        color: "#1b2224"
    }

    Drawer {
        id: drawer
        width: {
            if (platform.isAndroid) {
                parent.width / 4
            } else {
                200
            }
        }

        height: parent.height
        background: drawer_bg

        modal: inPortrait
        interactive: inPortrait || platform.isAndroid
        position: inPortrait ? 0 : 1
        visible: !inPortrait

        Column {
            anchors.fill: parent
            spacing: 5

            Text {
                text: "Tools"
                color: "white"
                font.pointSize: 12

                width: parent.width
                height: 40

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Rectangle {
                width: parent.width
                height: 2
                color: "#262f31"
            }

            SideMenuButton {
                tool_name: qsTr("Audio")
                icon_source: "../icons/menu/audio.png"

                onClicked: {
                    tool_label.text = qsTr("Current Tool: Audio")
                    swipe.setCurrentIndex(0)

                    if (inPortrait) {
                        drawer.close()
                    }
                }
            }

            SideMenuButton {
                tool_name: qsTr("Maps")
                icon_source: "../icons/menu/maps.png"

                onClicked: {
                    tool_label.text = qsTr("Current Tool: Maps")
                    swipe.setCurrentIndex(1)

                    if (inPortrait) {
                        drawer.close()
                    }
                }
            }

            SideMenuButton {
                tool_name: qsTr("Dice")
                icon_source: "../icons/menu/dice.png"

                onClicked: {
                    tool_label.text = qsTr("Current Tool: Dice")
                    swipe.setCurrentIndex(2)

                    if (inPortrait) {
                        drawer.close()
                    }
                }
            }

            SideMenuButton {
                tool_name: qsTr("Combat Tracker")
                icon_source: "../icons/menu/combat.png"

                onClicked: {
                    tool_label.text = qsTr("Current Tool: Combat Tracker")
                    swipe.setCurrentIndex(3)

                    if (inPortrait) {
                        drawer.close()
                    }
                }
            }

            SideMenuButton {
                tool_name: qsTr("Item Shop")
                icon_source: "../icons/menu/item-shop.png"

                onClicked: {
                    tool_label.text = qsTr("Current Tool: Item Shop")
                    swipe.setCurrentIndex(4)

                    if (inPortrait) {
                        drawer.close()
                    }
                }
            }

            SideMenuButton {
                tool_name: qsTr("Characters")
                icon_source: "../icons/menu/characters.png"

                onClicked: {
                    tool_label.text = qsTr("Current Tool: Characters")
                    swipe.setCurrentIndex(5)

                    if (inPortrait) {
                        drawer.close()
                    }
                }
            }

            SideMenuButton {
                tool_name: qsTr("Generators")
                icon_source: "../icons/menu/generators.png"

                onClicked: {
                    tool_label.text = qsTr("Current Tool: Generators")
                    swipe.setCurrentIndex(6)

                    if (inPortrait) {
                        drawer.close()
                    }
                }
            }

            SideMenuButton {
                tool_name: qsTr("Notes")
                icon_source: "../icons/menu/notes.png"

                onClicked: {
                    tool_label.text = qsTr("Current Tool: Notes")
                    swipe.setCurrentIndex(7)

                    if (inPortrait) {
                        drawer.close()
                    }
                }
            }

            SideMenuButton {
                tool_name: qsTr("Converter")
                icon_source: "../icons/menu/converter.png"

                onClicked: {
                    tool_label.text = qsTr("Current Tool: Unit Converter")
                    swipe.setCurrentIndex(8)

                    if (inPortrait) {
                        drawer.close()
                    }
                }
            }

            Rectangle {
                width: parent.width
                height: 2
                color: "#262f31"
            }

            SideMenuButton {
                tool_name: qsTr("Settings")
                icon_source: "../icons/menu/settings.png"

                onClicked: {
                    tool_label.text = qsTr("Settings")
                    swipe.setCurrentIndex(9)

                    if (inPortrait) {
                        drawer.close()
                    }
                }
            }
        }
    }

    SwipeView {
        id: swipe
        width: {

            if (inPortrait) {
                parent.width
            } else {
                if (drawer.visible) {
                    parent.width - drawer.width
                } else {
                    parent.width
                }
            }
        }

        height: parent.height - toolbar.height
        anchors.right: parent.right
        y: toolbar.height

        interactive: false
        currentIndex: 0

        Audio {
            id: audio
        }

        Maps {
            id: maps
        }

        Dice {
            id: dice
            padding: 5
        }

        CombatTracker {
            id: combat_tracker
        }

        ItemShop {
            id: item_shop
        }

        Characters {
            id: characters
        }

        Generators {
            id: generators
        }

        Notes {
            id: notes
        }

        Converter {
            id: converter
        }

        Settings {
            id: settings
        }
    }
}
