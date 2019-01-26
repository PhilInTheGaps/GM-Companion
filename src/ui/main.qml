import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import FontAwesome 2.0
import "./tools"
import "./components"
import "./main"

Window {
    id: window
    visible: true
    width: 1280
    height: 720
    title: qsTr("GM-Companion")

    readonly property bool inPortrait: window.width < window.height

    Component.onCompleted: {
        if (settings_tool.getCheckForUpdates())
            update_manager.checkForUpdates()
    }

    // Loading screen
    LoadingScreen {
        id: splash
    }

    Connections {
        target: update_manager

        onUpdateAvailable: {
            update_dialog.open()
        }
    }

    // Update popup
    Dialog {
        id: update_dialog

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        title: qsTr("Update Available!")

        Button {
            text: qsTr("Download")

            onClicked: Qt.openUrlExternally(
                           "https://github.com/PhilInTheGaps/GM-Companion/releases")
        }
    }

    // Main item
    Page {
        id: main_item
        anchors.fill: parent

        // Menu
        Drawer {
            id: drawer
            width: {
                if (platform.isAndroid) {
                    inPortrait
                            || !settings_tool.showToolNames ? color_scheme.toolbarHeight : parent.width / 4
                } else {
                    inPortrait
                            || !settings_tool.showToolNames ? color_scheme.toolbarHeight : 200
                }
            }

            height: parent.height
            background: Rectangle {
                color: color_scheme.menuColor
            }

            modal: false
            interactive: false
            visible: true

            // Contains all the menu stuff
            ScrollView {
                clip: true
                contentWidth: -1
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: settings_button.top

                ScrollBar.vertical.visible: false

                Column {
                    width: drawer.width

                    Rectangle {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: color_scheme.toolbarHeight

                        // Tools Text
                        Text {
                            text: "Tools"
                            color: color_scheme.toolbarTextColor
                            font.pointSize: 14
                            visible: !inPortrait && settings_tool.showToolNames

                            anchors.centerIn: parent

                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        // GM-Companion Icon
                        Image {
                            anchors.centerIn: parent
                            visible: inPortrait || !settings_tool.showToolNames

                            width: parent.width - 10
                            height: width

                            source: "../icons/gm-companion/icon.png"
                            sourceSize.width: width
                            sourceSize.height: height
                        }

                        color: color_scheme.toolbarColor
                    }

                    // Column with tool buttons
                    Column {
                        id: tool_column
                        anchors.left: parent.left
                        anchors.right: parent.right
                        spacing: 5

                        // Top divider
                        Rectangle {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            height: 2
                            color: color_scheme.dividerColor
                        }

                        function buttonClicked(tool) {
                            if (stack.currentItem !== tool) {
                                stack.pop(null)
                                tool.active = true
                                stack.push(tool)
                            }
                        }

                        property var toolNames: [qsTr("Audio"), qsTr(
                                "Maps"), qsTr("Dice"), qsTr(
                                "Combat Tracker"), qsTr("Item Shop"), qsTr(
                                "Characters"), qsTr("Generators"), qsTr(
                                "Notes"), qsTr("Converter")]
                        property var icons: ["../icons/menu/audio.png", "../icons/menu/maps.png", "../icons/menu/dice.png", "../icons/menu/combat.png", "../icons/menu/item-shop.png", "../icons/menu/characters.png", "../icons/menu/generators.png", "../icons/menu/notes.png", "../icons/menu/converter.png"]
                        property var tools: [audio, maps, dice, combat, shop, characters, generators, notes, converter]
                        property var fa_icons: [FontAwesome.music, FontAwesome.mapMarkedAlt, FontAwesome.diceD20, FontAwesome.bookDead, FontAwesome.shoppingCart, FontAwesome.addressCard, FontAwesome.industry, FontAwesome.book, FontAwesome.balanceScale]

                        Repeater {
                            model: parent.toolNames

                            SideMenuButton {
                                tool_name: modelData
                                icon_source: parent.icons[index]
                                fa_icon: parent.fa_icons[index]
                                current_tool: stack.currentItem == tool_column.tools[index]

                                onClicked: {
                                    if (index == 0) {
                                        if (stack.currentItem !== audio) {
                                            stack.pop(null)
                                        }
                                    } else {
                                        tool_column.buttonClicked(
                                                    tool_column.tools[index])
                                    }
                                }
                            }
                        }
                    }
                }
            }

            SideMenuButton {
                id: settings_button
                anchors.bottom: parent.bottom

                tool_name: qsTr("Settings")
                icon_source: "../icons/menu/settings.png"
                fa_icon: FontAwesome.cog
                current_tool: stack.currentItem == settings

                onClicked: tool_column.buttonClicked(settings)
            }
        }

        // Stackview contains current tool
        StackView {
            id: stack
            width: {
                if (inPortrait || platform.isAndroid) {
                    parent.width - drawer.width
                } else {
                    parent.width - drawer.width
                }
            }

            clip: true

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right

            background: Rectangle {
                color: color_scheme.backgroundColor
            }

            initialItem: audio
        }

        Loader {
            id: audio
            source: "tools/Audio.qml"
            asynchronous: true
            active: true
        }

        Loader {
            id: maps
            source: "tools/Maps.qml"
            asynchronous: true
            active: false
        }

        Loader {
            source: "tools/Dice.qml"
            asynchronous: true
            id: dice
            active: false
        }

        Loader {
            source: "tools/CombatTracker.qml"
            asynchronous: true
            id: combat
            active: false
        }

        Loader {
            source: "tools/ItemShop.qml"
            asynchronous: true
            id: shop
            active: false
        }

        Loader {
            source: "tools/Characters.qml"
            asynchronous: true
            id: characters
            active: false
        }

        Loader {
            source: "tools/Generators.qml"
            asynchronous: true
            id: generators
            active: false
        }

        Loader {
            source: "tools/Notes.qml"
            asynchronous: true
            id: notes
            active: false
        }

        Loader {
            source: "tools/Converter.qml"
            asynchronous: true
            id: converter
            active: false
        }
        Loader {
            source: "tools/Settings.qml"
            asynchronous: true
            id: settings
            active: false
        }
    }
}
