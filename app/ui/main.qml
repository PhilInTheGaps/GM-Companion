import QtQuick
import QtQuick.Controls
import FontAwesome 2.0
import "./tools"
import "./main"
import "defines.js" as Defines

ApplicationWindow {
    id: main_window
    title: qsTr("GM-Companion")
    visible: true

    palette.text: "#f6f7f8"
    palette.base: "#2e2e2e"
    palette.alternateBase: "#343a43"
    palette.window: "#2e2e2e"
    palette.windowText: "#f6f7f8"
    palette.button: "#3f4957"
    palette.buttonText: "#f6f7f8"
    palette.light: "#ebedef"
    palette.midlight: "#d5dade"
    palette.mid: "#bfc6cd"
    palette.dark: "#1f1f1f"
    palette.highlight: "#3f4957"
    palette.highlightedText: "#f6f7f8"
    palette.toolTipText: "#f6f7f8"
    palette.toolTipBase: "#1f1f1f"

    width: 1280
    height: 720

    minimumWidth: 640
    minimumHeight: 480

    signal zoomIn
    signal zoomOut
    signal save

    readonly property var tools: [{
            "name": qsTr("Audio"),
            "source": "tools/Audio.qml",
            "classicIcon": "../icons/audio.png",
            "faIcon": FontAwesome.music
        }, {
            "name": qsTr("Maps"),
            "source": "tools/Maps.qml",
            "classicIcon": "../icons/maps.png",
            "faIcon": FontAwesome.mapMarkedAlt
        }, {
            "name": qsTr("Dice"),
            "source": "tools/Dice.qml",
            "classicIcon": "../icons/dice.png",
            "faIcon": FontAwesome.diceD20
        }, {
            "name": qsTr("Combat Tracker"),
            "source": "tools/CombatTracker.qml",
            "classicIcon": "../icons/combat.png",
            "faIcon": FontAwesome.bookDead
        }, {
            "name": qsTr("Item Shop"),
            "source": "tools/ItemShop.qml",
            "classicIcon": "../icons/item-shop.png",
            "faIcon": FontAwesome.shoppingCart
        }, {
            "name": qsTr("Characters"),
            "source": "tools/Characters.qml",
            "classicIcon": "../icons/characters.png",
            "faIcon": FontAwesome.addressCard
        }, {
            "name": qsTr("Generators"),
            "source": "tools/Generators.qml",
            "classicIcon": "../icons/generators.png",
            "faIcon": FontAwesome.industry
        }, {
            "name": qsTr("Notes"),
            "source": "tools/Notes.qml",
            "classicIcon": "../icons/notes.png",
            "faIcon": FontAwesome.book
        }, {
            "name": qsTr("Converter"),
            "source": "tools/Converter.qml",
            "classicIcon": "../icons/converter.png",
            "faIcon": FontAwesome.balanceScale
        }]

    Component.onCompleted: {
        if (settings_manager.checkForUpdates) {
            update_manager.checkForUpdates()
        }

        if (!settings_manager.has("crashReports", "Telemetry")) {
            console.debug("CrashReports preference has not been set.")
            new_settings_dialog.open()
        }
    }

    UpdateDialog {
        id: update_dialog

        Connections {
            target: update_manager

            function onUpdateAvailable() {
                update_dialog.open()
            }
        }
    }

    NewSettingsDialog {
        id: new_settings_dialog
    }

    MessageDialog {
        id: message_dialog
        width: main_window.width - 100
        height: main_window.height - 100
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
                    model: main_window.tools

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
            id: message_button
            anchors.bottom: settings_button.top

            toolName: qsTr("Messages")
            faIcon: FontAwesome.exclamationCircle
            altColor: "orange"
            useAltColor: message_manager.hasNewErrors

            visible: message_manager.messages.length > 0

            onClicked: {

                if (message_dialog.opened) {
                    message_dialog.close()
                } else {
                    message_dialog.open()
                    message_manager.markAllAsRead()
                }
            }
        }

        SideMenuButton {
            id: settings_button
            anchors.bottom: parent.bottom

            toolName: qsTr("Settings")
            iconSource: "../icons/settings.png"
            faIcon: FontAwesome.cog

            onClicked: {

                loader.setSource("tools/Settings.qml")
            }
        }
    }

    Loader {
        id: loader
        width: parent.width - drawer.width
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        asynchronous: true
        source: main_window.tools[0].source
        active: true

        onLoaded: splash.close()
    }


    /**
     * Keyboard Shortcuts
     */
    Shortcut {
        sequences: [StandardKey.ZoomIn]
        onActivated: zoomIn()
        context: Qt.ApplicationShortcut
    }

    Shortcut {
        sequences: [StandardKey.ZoomOut]
        onActivated: zoomOut()
        context: Qt.ApplicationShortcut
    }

    Shortcut {
        sequences: [StandardKey.Save]
        onActivated: save()
        context: Qt.ApplicationShortcut
    }
}
