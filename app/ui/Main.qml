pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import IconFonts
import common
import src
import "./main"

ApplicationWindow {
    id: root
    title: qsTr("GM-Companion")
    visible: true

    width: 1280
    height: 720

    minimumWidth: 640
    minimumHeight: 480

    property int currentToolIndex: 0

    readonly property var tools: [{
            "name": qsTr("Audio"),
            "source": "tools/Audio.qml",
            "faIcon": FontAwesome.music
        }, {
            "name": qsTr("Maps"),
            "source": "tools/Maps.qml",
            "faIcon": FontAwesome.mapLocationDot
        }, {
            "name": qsTr("Dice"),
            "source": "tools/Dice.qml",
            "faIcon": FontAwesome.diceD20
        }, {
            "name": qsTr("Combat Tracker"),
            "source": "tools/CombatTracker.qml",
            "faIcon": FontAwesome.bookSkull
        }, {
            "name": qsTr("Item Shop"),
            "source": "tools/ItemShop.qml",
            "faIcon": FontAwesome.cartShopping
        }, {
            "name": qsTr("Characters"),
            "source": "tools/Characters.qml",
            "faIcon": FontAwesome.addressCard
        }, {
            "name": qsTr("Generators"),
            "source": "tools/Generators.qml",
            "faIcon": FontAwesome.industry
        }, {
            "name": qsTr("Notes"),
            "source": "tools/Notes.qml",
            "faIcon": FontAwesome.book
        }, {
            "name": qsTr("Converter"),
            "source": "tools/Converter.qml",
            "faIcon": FontAwesome.scaleBalanced
        }]

    Connections {
        target: UpdateManager

        function onUpdateAvailable() {
            update_dialog.open()
        }
    }

    UpdateDialog {
        id: update_dialog
    }

    NewSettingsDialog {
        id: new_settings_dialog
    }

    Component.onCompleted: {
        if (SettingsManager.checkForUpdates) {
            UpdateManager.checkForUpdates()
        }

        if (!SettingsManager.has("crashReports", "Telemetry")) {
            console.debug("CrashReports preference has not been set.")
            new_settings_dialog.open()
        }
    }

    MessageDialog {
        id: message_dialog
        width: root.width - 100
        height: root.height - 100
    }

    LoadingScreen {
        id: splash
        progress: loader.progress
    }

    Drawer {
        id: drawer
        width: SettingsManager.showToolNames ? Sizes.sidebarWidth : Sizes.toolbarWidth
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

                Repeater {
                    model: root.tools

                    SideMenuButton {
                        id: side_menu_button
                        required property var modelData
                        required property int index

                        toolName: modelData.name
                        faIcon: modelData.faIcon

                        Shortcut {
                            onActivated: side_menu_button.setTool()
                            sequences: ["Ctrl+" + (side_menu_button.index + 1)]
                            context: Qt.ApplicationShortcut
                        }

                        onClicked: setTool()

                        function setTool() {
                            root.currentToolIndex = index
                        }
                    }
                }
            }
        }

        SideMenuButton {
            id: message_button
            anchors.bottom: settings_button.top

            toolName: qsTr("Messages")
            faIcon: FontAwesome.circleExclamation
            altColor: "orange"
            useAltColor: MessageManager.hasNewErrors

            visible: message_dialog.hasMessages

            onClicked: {
                if (message_dialog.opened) {
                    message_dialog.close()
                } else {
                    message_dialog.open()
                    MessageManager.markAllAsRead()
                }
            }
        }

        SideMenuButton {
            id: settings_button
            anchors.bottom: parent.bottom

            toolName: qsTr("Settings")
            faIcon: FontAwesome.gear

            onClicked: {
                root.currentToolIndex = -1
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
        source: root.currentToolIndex
                < 0 ? "tools/Settings.qml" : root.tools[root.currentToolIndex].source
        active: true

        onLoaded: splash.close()
    }
}
