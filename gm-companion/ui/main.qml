import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import "./tools"
import "./menu"
import gm.companion.platforms 1.0
import gm.companion.colorscheme 1.0

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

    ColorScheme {
        id: color_scheme
    }

    Popup {
        id: splash
        width: parent.width
        height: parent.height

        background: Rectangle {
            color: "#222222"
        }

        visible: {
            audio.status !== Loader.Ready && maps.status !== Loader.Ready && dice.status
                    !== Loader.Ready && combat.status !== Loader.Ready && shop.status
                    !== Loader.Ready && characters.status !== Loader.Ready && generators.status
                    !== Loader.Ready && notes.status !== Loader.Ready && converter.status
                    !== Loader.Ready && settings.status !== Loader.Ready
        }

        Column {
            anchors.centerIn: parent
            spacing: 5

            Image {
                source: "/splash.jpg"
            }

            BusyIndicator {
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                text: qsTr("Loading ...")
                color: "white"
            }
        }
    }

    Item {
        id: main_item
        anchors.fill: parent

        ToolBar {
            id: toolbar
            width: {
                platform.isAndroid ? parent.width : inPortrait ? parent.width : drawer.opened ? parent.width - drawer.width : parent.width
            }

            height: platform.isAndroid ? window.height / 15 : (inPortrait ? window.height / 24 : 0)
            visible: platform.isAndroid || inPortrait ? true : false
            anchors.right: parent.right

            background: Rectangle {
                id: toolbar_bg
                color: color_scheme.toolbarColor
            }

            Row {
                id: toolbar_row_left
                height: parent.height

                ToolButton {
                    id: tools_button

                    Image {
                        anchors.centerIn: parent
                        width: parent.width - 10
                        height: parent.height - 10
                        source: "/icons/menu/three_bars_white.png"
                    }

                    background: Rectangle {
                        color: "transparent"
                    }

                    height: parent.height
                    width: height
                    visible: !drawer.opened || inPortrait ? true : false

                    onClicked: drawer.open()
                }
            }

            Label {
                id: tool_label
                text: "Current Tool: Audio"

                color: color_scheme.toolbarTextColor
                height: parent.height
                verticalAlignment: "AlignVCenter"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: toolbar_row_left.verticalCenter
            }
        }

        Drawer {
            id: drawer
            width: {
                if (platform.isAndroid) {
                    inPortrait ? parent.width / 2 : parent.width / 4
                } else {
                    inPortrait ? parent.width / 3 : 200
                }
            }

            height: parent.height
            background: Rectangle {
                color: color_scheme.menuColor
            }

            modal: inPortrait || platform.isAndroid
            interactive: inPortrait || platform.isAndroid
            position: inPortrait ? 0 : platform.isAndroid ? 0 : 1
            visible: !inPortrait || !platform.isAndroid

            ScrollView {
                width: parent.width
                height: parent.height
                clip: true
                contentWidth: -1

                Column {
                    width: drawer.width
                    spacing: 5

                    Rectangle {
                        width: parent.width
                        height: toolbar.visible ? toolbar.height
                                                  - parent.spacing : window.height / 24

                        Text {
                            text: "Tools"
                            color: color_scheme.toolbarTextColor
                            font.pointSize: 12

                            anchors.fill: parent

                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        color: color_scheme.toolbarColor
                    }

                    Rectangle {
                        width: parent.width
                        height: 2
                        color: color_scheme.dividerColor
                    }

                    TabBar {
                    }

                    SideMenuButton {
                        tool_name: qsTr("Audio")
                        icon_source: "../icons/menu/audio.png"

                        onClicked: {
                            tool_label.text = qsTr("Current Tool: Audio")
                            swipe.setCurrentIndex(0)

                            if (inPortrait || platform.isAndroid) {
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

                            if (inPortrait || platform.isAndroid) {
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

                            if (inPortrait || platform.isAndroid) {
                                drawer.close()
                            }
                        }
                    }

                    SideMenuButton {
                        tool_name: qsTr("Combat Tracker")
                        icon_source: "../icons/menu/combat.png"

                        onClicked: {
                            tool_label.text = qsTr(
                                        "Current Tool: Combat Tracker")
                            swipe.setCurrentIndex(3)

                            if (inPortrait || platform.isAndroid) {
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

                            if (inPortrait || platform.isAndroid) {
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

                            if (inPortrait || platform.isAndroid) {
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

                            if (inPortrait || platform.isAndroid) {
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

                            if (inPortrait || platform.isAndroid) {
                                drawer.close()
                            }
                        }
                    }

                    SideMenuButton {
                        tool_name: qsTr("Converter")
                        icon_source: "../icons/menu/converter.png"

                        onClicked: {
                            tool_label.text = qsTr(
                                        "Current Tool: Unit Converter")
                            swipe.setCurrentIndex(8)

                            if (inPortrait || platform.isAndroid) {
                                drawer.close()
                            }
                        }
                    }

                    Rectangle {
                        width: parent.width
                        height: 2
                        color: color_scheme.dividerColor
                    }

                    SideMenuButton {
                        tool_name: qsTr("Settings")
                        icon_source: "../icons/menu/settings.png"

                        onClicked: {
                            tool_label.text = qsTr("Settings")
                            swipe.setCurrentIndex(9)

                            if (inPortrait || platform.isAndroid) {
                                drawer.close()
                            }
                        }
                    }
                }
            }
        }

        SwipeView {
            id: swipe
            width: {

                if (inPortrait || platform.isAndroid) {
                    parent.width
                } else {
                    parent.width - drawer.width
                }
            }

            height: parent.height - toolbar.height
            anchors.right: parent.right
            y: toolbar.height

            background: Rectangle {
                color: color_scheme.backgroundColor
            }

            clip: true
            interactive: false
            currentIndex: 0

            Loader {
                id: audio
                source: "tools/Audio.qml"
                asynchronous: true
            }

            Loader {
                id: maps
                source: "tools/Maps.qml"
                asynchronous: true
            }

            Loader {
                source: "tools/Dice.qml"
                asynchronous: true
                id: dice
            }

            Loader {
                source: "tools/CombatTracker.qml"
                asynchronous: true
                id: combat
            }

            Loader {
                source: "tools/ItemShop.qml"
                asynchronous: true
                id: shop
            }

            Loader {
                source: "tools/Characters.qml"
                asynchronous: true
                id: characters
            }

            Loader {
                source: "tools/Generators.qml"
                asynchronous: true
                id: generators
            }

            Loader {
                source: "tools/Notes.qml"
                asynchronous: true
                id: notes
            }

            Loader {
                source: "tools/Converter.qml"
                asynchronous: true
                id: converter
            }

            Loader {
                source: "tools/Settings.qml"
                asynchronous: true
                id: settings
            }
        }
    }
}
