import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import "./tools"
import "./menu"
import gm.companion.platforms 1.0
import gm.companion.colorscheme 1.0
import gm.companion.settingstool 1.0
import gm.companion.updatemanager 1.0

Window {
    id: window
    visible: true
    width: 1280
    height: 720
    title: qsTr("GM-Companion")

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

        visible: audio.status !== Loader.Ready

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

    SettingsTool {
        id: settings_tool
    }

    UpdateManager {
        id: update_manager

        Component.onCompleted: {
            setCurrentVersion(1000)

            if (settings_tool.getCheckForUpdates())
                checkForUpdates()
        }

        onUpdateAvailable: {
            update_dialog.open()
        }
    }

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

    Item {
        id: main_item
        anchors.fill: parent

        ToolBar {
            id: toolbar
            width: {
                platform.isAndroid ? parent.width : inPortrait ? parent.width : drawer.opened ? parent.width - drawer.width : parent.width
            }

            height: color_scheme.toolbarHeight

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
                anchors.fill: parent
                clip: true
                contentWidth: -1

                Column {
                    width: drawer.width

                    Rectangle {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: color_scheme.toolbarHeight

                        Text {
                            text: "Tools"
                            color: color_scheme.toolbarTextColor
                            font.pointSize: 14

                            anchors.centerIn: parent

                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        color: color_scheme.toolbarColor
                    }

                    Column {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        spacing: 5

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

                            if (inPortrait || platform.isAndroid) {
                                drawer.close()
                            }
                        }

                        SideMenuButton {
                            tool_name: qsTr("Audio")
                            icon_source: "../icons/menu/audio.png"

                            onClicked: {
                                tool_label.text = qsTr("Current Tool: Audio")

                                if (stack.currentItem !== audio) {
                                    stack.pop(null)
                                }

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
                                parent.buttonClicked(maps)
                            }
                        }

                        SideMenuButton {
                            tool_name: qsTr("Dice")
                            icon_source: "../icons/menu/dice.png"

                            onClicked: {
                                tool_label.text = qsTr("Current Tool: Dice")
                                parent.buttonClicked(dice)
                            }
                        }

                        SideMenuButton {
                            tool_name: qsTr("Combat Tracker")
                            icon_source: "../icons/menu/combat.png"

                            onClicked: {
                                tool_label.text = qsTr(
                                            "Current Tool: Combat Tracker")
                                parent.buttonClicked(combat)
                            }
                        }

                        SideMenuButton {
                            tool_name: qsTr("Item Shop")
                            icon_source: "../icons/menu/item-shop.png"

                            onClicked: {
                                tool_label.text = qsTr(
                                            "Current Tool: Item Shop")
                                parent.buttonClicked(shop)
                            }
                        }

                        SideMenuButton {
                            tool_name: qsTr("Characters")
                            icon_source: "../icons/menu/characters.png"

                            onClicked: {
                                tool_label.text = qsTr(
                                            "Current Tool: Characters")
                                parent.buttonClicked(characters)
                            }
                        }

                        SideMenuButton {
                            tool_name: qsTr("Generators")
                            icon_source: "../icons/menu/generators.png"

                            onClicked: {
                                tool_label.text = qsTr(
                                            "Current Tool: Generators")
                                parent.buttonClicked(generators)
                            }
                        }

                        SideMenuButton {
                            tool_name: qsTr("Notes")
                            icon_source: "../icons/menu/notes.png"

                            onClicked: {
                                tool_label.text = qsTr("Current Tool: Notes")
                                parent.buttonClicked(notes)
                            }
                        }

                        SideMenuButton {
                            tool_name: qsTr("Converter")
                            icon_source: "../icons/menu/converter.png"

                            onClicked: {
                                tool_label.text = qsTr(
                                            "Current Tool: Unit Converter")
                                parent.buttonClicked(converter)
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
                                parent.buttonClicked(settings)
                            }
                        }
                    }
                }
            }
        }

        StackView {
            id: stack
            width: {
                if (inPortrait || platform.isAndroid) {
                    parent.width
                } else {
                    parent.width - drawer.width
                }
            }

            anchors.top: toolbar.visible ? toolbar.bottom : parent.top
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
