import QtQuick 2.9
import QtQuick.Controls 2.2

import FontAwesome 2.0
import "./tools"
import "./main"

import "defines.js" as Defines

ApplicationWindow {
    id: root
    title: qsTr("GM-Companion")
    visible: true

    width: 1280
    height: 720

    minimumWidth: 640
    minimumHeight: 480

    readonly property var tools: [{
            "name": qsTr("Audio"),
            "source": "tools/Audio.qml",
            "classicIcon": "../icons/menu/audio.png",
            "faIcon": FontAwesome.music
        }, {
            "name": qsTr("Maps"),
            "source": "tools/Maps.qml",
            "classicIcon": "../icons/menu/maps.png",
            "faIcon": FontAwesome.mapMarkedAlt
        }, {
            "name": qsTr("Dice"),
            "source": "tools/Dice.qml",
            "classicIcon": "../icons/menu/dice.png",
            "faIcon": FontAwesome.diceD20
        }, {
            "name": qsTr("Combat Tracker"),
            "source": "tools/CombatTracker.qml",
            "classicIcon": "../icons/menu/combat.png",
            "faIcon": FontAwesome.bookDead
        }, {
            "name": qsTr("Item Shop"),
            "source": "tools/ItemShop.qml",
            "classicIcon": "../icons/menu/item-shop.png",
            "faIcon": FontAwesome.shoppingCart
        }, {
            "name": qsTr("Characters"),
            "source": "tools/Characters.qml",
            "classicIcon": "../icons/menu/characters.png",
            "faIcon": FontAwesome.addressCard
        }, {
            "name": qsTr("Generators"),
            "source": "tools/Generators.qml",
            "classicIcon": "../icons/menu/generators.png",
            "faIcon": FontAwesome.industry
        }, {
            "name": qsTr("Notes"),
            "source": "tools/Notes.qml",
            "classicIcon": "../icons/menu/notes.png",
            "faIcon": FontAwesome.book
        }, {
            "name": qsTr("Converter"),
            "source": "tools/Converter.qml",
            "classicIcon": "../icons/menu/converter.png",
            "faIcon": FontAwesome.balanceScale
        }]

    Component.onCompleted: {
        if (settings_manager.isUpdateCheckEnabled()) {
            console.debug("Checking for updates")
            update_manager.checkForUpdates()
        }
    }

    UpdateDialog {
        id: update_dialog

        Connections {
            target: update_manager

            onUpdateAvailable: update_dialog.open()
        }
    }

    LoadingScreen {
        id: splash
        progress: loader.progress
    }

    Drawer {
        id: drawer
        width: settings_manager.showToolNames ? Defines.SIDEBAR_WIDTH : Defines.TOOLBAR_WIDTH
        height: parent.height
        modal: false
        interactive: false
        visible: true

        background: Rectangle {
            color: palette.alternateBase
        }

        ScrollView {
            clip: true
            contentWidth: -1
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: settings_button.top

            ScrollBar.vertical.visible: false
            contentHeight: tool_column.implicitHeight

            Column {
                id: tool_column
                width: drawer.width

                function buttonClicked(tool) {
                    if (stack.currentItem !== tool) {
                        stack.pop(null)
                        tool.active = true
                        stack.push(tool)
                    }
                }

                Repeater {
                    model: root.tools

                    SideMenuButton {
                        toolName: modelData.name
                        iconSource: modelData.classicIcon
                        faIcon: modelData.faIcon

                        Shortcut {
                            onActivated: setTool()
                            sequences: ["Ctrl+" + (index + 1)]
                            context: Qt.ApplicationShortcut
                        }

                        onClicked: setTool()

                        function setTool() {
                            loader.setSource(modelData.source)
                        }
                    }
                }
            }
        }

        SideMenuButton {
            id: settings_button
            anchors.bottom: parent.bottom

            toolName: qsTr("Settings")
            iconSource: "../icons/menu/settings.png"
            faIcon: FontAwesome.cog

            onClicked: loader.setSource("tools/Settings.qml")
        }
    }

    Loader {
        id: loader
        width: parent.width - drawer.width
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        asynchronous: true
        source: root.tools[0].source
        active: true

        onLoaded: splash.close()
    }
}
