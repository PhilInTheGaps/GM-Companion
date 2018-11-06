import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.3

import gm.companion.platforms 1.0
import gm.companion.settingstool 1.0
import gm.companion.updatemanager 1.0
import gm.companion.colorscheme 1.0
import gm.companion.addonmanager 1.0
import "./settings"

Page {
    id: settings_page

    SettingsTool {
        id: settings_tool
    }

    UpdateManager {
        id: update_manager
        Component.onCompleted: {
            addon_manager.updateAddonList()

            if (settings_tool.getCheckForUpdates())
                checkForUpdates()
        }

        onUpdateAvailable: {
            update_text.text = qsTr("Found new Version: ") + newestVersion
            update_text.visible = true
            open_downloads_button.visible = true
            update_busy_indicator.visible = false
        }

        onNoUpdateAvailable: {
            update_text.text = qsTr("No newer version found")
            update_text.visible = true
            update_busy_indicator.visible = false
        }
    }

    AddonManager {
        id: addon_manager

        onAddonListChanged: {
            addon_column.children = []

            var component = Qt.createComponent("./settings/AddonItem.qml")

            for (var i = 0; i < getAddonNames().length; i++) {

                var addonItem = component.createObject(addon_column, {
                                                           "addon": getAddonNames(
                                                                        )[i],
                                                           "description": getAddonDescriptions()[i],
                                                           "folder": getAddonPathNames(
                                                                         )[i],
                                                           "addon_enabled": getAddonEnabledList()[i]
                                                       })
            }
        }
    }

    PlatformDetails {
        id: platform_details
    }

    ColorScheme {
        id: color_scheme
    }

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    FileDialog {
        id: file_dialog

        property var text_field

        selectFolder: true

        onAccepted: {
            if (platform_details.isWindows)
                text_field.text = fileUrl.toString().replace("file:///", "")
            else
                text_field.text = fileUrl.toString().replace("file://", "")
        }
    }

    // Header
    Column {
        anchors.fill: parent
        bottomPadding: 5

        TabBar {
            id: tab_bar
            width: parent.width - parent.padding * 2
            height: color_scheme.toolbarHeight

            TabButton {
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter

                Text {
                    text: qsTr("General")
                    color: tab_bar.currentIndex == 0 ? "black" : color_scheme.toolbarTextColor
                    font.pointSize: 12
                    font.bold: true
                    anchors.centerIn: parent
                }

                background: Rectangle {
                    color: tab_bar.currentIndex == 0 ? "white" : color_scheme.toolbarColor
                }
            }

            TabButton {
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter

                Text {
                    text: qsTr("Paths")
                    color: tab_bar.currentIndex == 1 ? "black" : color_scheme.toolbarTextColor
                    font.pointSize: 12
                    font.bold: true
                    anchors.centerIn: parent
                }

                background: Rectangle {
                    color: tab_bar.currentIndex == 1 ? "white" : color_scheme.toolbarColor
                }
            }

            TabButton {
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter

                Text {
                    text: qsTr("RPG Addons")
                    color: tab_bar.currentIndex == 2 ? "black" : color_scheme.toolbarTextColor
                    font.pointSize: 12
                    font.bold: true
                    anchors.centerIn: parent
                }

                background: Rectangle {
                    color: tab_bar.currentIndex == 2 ? "white" : color_scheme.toolbarColor
                }
            }

            TabButton {
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter

                Text {
                    text: qsTr("Info")
                    color: tab_bar.currentIndex == 3 ? "black" : color_scheme.toolbarTextColor
                    font.pointSize: 12
                    font.bold: true
                    anchors.centerIn: parent
                }

                background: Rectangle {
                    color: tab_bar.currentIndex == 3 ? "white" : color_scheme.toolbarColor
                }
            }
        }

        // Main
        SwipeView {
            id: swipe_view
            width: parent.width - parent.padding * 2
            height: parent.height - parent.spacing * 2 - parent.bottomPadding
                    - tab_bar.height - restart_info_text.height
            currentIndex: tab_bar.currentIndex
            padding: 5
            spacing: 5
            interactive: false

            // General Settings
            Column {
                id: general_column

                spacing: 5

                Text {
                    text: qsTr("Language")
                    font.bold: true
                    color: color_scheme.textColor
                }

                Row {
                    width: parent.width

                    Text {
                        text: qsTr("Select Language")
                        width: platform_details.isAndroid ? settings_page.width
                                                            / 4 : language_box.width
                        anchors.verticalCenter: parent.verticalCenter
                        color: color_scheme.textColor
                    }

                    ComboBox {
                        id: language_box
                        property bool loaded: false
                        model: ["English", "Deutsch"]
                        width: settings_page.width / 6

                        onCurrentTextChanged: {
                            if (loaded) {
                                settings_tool.setLanguage(currentText)
                            }
                        }

                        Component.onCompleted: {
                            currentIndex = settings_tool.currentLanguageIndex
                            loaded = true
                        }
                    }
                }

                Rectangle {
                    height: 10
                    width: height
                    color: "transparent"
                }

                Text {
                    text: qsTr("User Interface")
                    font.bold: true
                    color: color_scheme.textColor
                }

                // UI Style
                Row {
                    width: parent.width

                    Text {
                        text: qsTr("Select Style")
                        width: platform_details.isAndroid ? settings_page.width
                                                            / 4 : language_box.width
                        anchors.verticalCenter: parent.verticalCenter
                        color: color_scheme.textColor
                    }

                    ComboBox {
                        id: style_box
                        property bool loaded: false
                        model: ["Dark", "Bright"]
                        width: settings_page.width / 6

                        onCurrentTextChanged: {
                            if (loaded) {
                                settings_tool.setUiStyle(currentText)
                            }
                        }

                        Component.onCompleted: {
                            currentIndex = settings_tool.uiStyleIndex
                            loaded = true
                        }
                    }
                }

                // Alternative Menu
                Row {
                    CheckBox {
                        checked: settings_tool.getAltMenu()

                        onCheckedChanged: {
                            settings_tool.setAltMenu(checked)
                        }
                    }

                    Text {
                        text: qsTr("Alternative Menu")
                        color: color_scheme.textColor
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                Rectangle {
                    height: 10
                    width: height
                    color: "transparent"
                }

                Text {
                    text: qsTr("Updates")
                    font.bold: true
                    visible: platform_details.isAndroid ? false : true
                    color: color_scheme.textColor
                }

                Row {
                    width: parent.width
                    spacing: 5

                    Button {
                        id: check_for_updates_button
                        text: qsTr("Check for Updates")
                        visible: platform_details.isAndroid ? false : true

                        onClicked: {
                            update_text.visible = false
                            update_busy_indicator.visible = true
                            update_manager.checkForUpdates()
                        }
                    }

                    BusyIndicator {
                        id: update_busy_indicator
                        visible: false
                        width: check_for_updates_button.height
                        height: width
                    }

                    Text {
                        id: update_text
                        anchors.verticalCenter: parent.verticalCenter
                        visible: false
                        color: "green"
                    }
                }

                Row {
                    CheckBox {
                        checked: settings_tool.getCheckForUpdates()

                        onCheckedChanged: {
                            settings_tool.setCheckForUpdates(checked)
                        }
                    }

                    Text {
                        text: qsTr("Automatically check for updates")
                        color: color_scheme.textColor
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                Button {
                    id: open_downloads_button
                    visible: false
                    text: qsTr("Open Download Page")

                    onClicked: {
                        Qt.openUrlExternally(
                                    "https://github.com/PhilInTheGaps/GM-Companion/releases")
                    }
                }

                Rectangle {
                    height: 10
                    width: height
                    color: "transparent"
                }

                Text {
                    text: qsTr("Spotify (Experimental Feature)")
                    font.bold: true
                    visible: platform_details.isAndroid ? false : true
                    color: color_scheme.textColor
                }

                Row {
                    spacing: 10
                    Text {
                        text: qsTr("Client ID")
                        width: platform_details.isAndroid ? settings_page.width
                                                            / 4 : language_box.width
                        anchors.verticalCenter: parent.verticalCenter
                        color: color_scheme.textColor
                    }

                    TextField {
                        selectByMouse: true
                        onTextChanged: settings_tool.setSpotifyID(text)
                        width: settings_page.width / 6

                        Component.onCompleted: text = settings_tool.getSpotifyID()
                    }
                }

                Row {
                    spacing: 10
                    Text {
                        text: qsTr("Client Secret")
                        width: platform_details.isAndroid ? settings_page.width
                                                            / 4 : language_box.width
                        anchors.verticalCenter: parent.verticalCenter
                        color: color_scheme.textColor
                    }

                    TextField {
                        selectByMouse: true
                        onTextChanged: settings_tool.setSpotifySecret(text)
                        width: settings_page.width / 6

                        Component.onCompleted: text = settings_tool.getSpotifySecret()
                    }
                }

                Text {
                    text: qsTr("Requires Spotify Premium")
                    visible: platform_details.isAndroid ? false : true
                    color: color_scheme.textColor
                }
            }

            // Paths
            Column {
                spacing: 5

                PathItem {
                    path_type: qsTr("Audio Projects")
                    setting: "audio"
                    width: parent.width
                }

                PathItem {
                    path_type: qsTr("Music")
                    setting: "music"
                    width: parent.width
                }

                PathItem {
                    path_type: qsTr("Sound")
                    setting: "sound"
                    width: parent.width
                }

                PathItem {
                    path_type: qsTr("Internet Radio Playlists")
                    setting: "radio"
                    width: parent.width
                }

                PathItem {
                    path_type: qsTr("Maps")
                    setting: "maps"
                    width: parent.width
                }

                PathItem {
                    path_type: qsTr("Characters")
                    setting: "characters"
                    width: parent.width
                }

                PathItem {
                    path_type: qsTr("Notes")
                    setting: "notes"
                    width: parent.width
                }

                PathItem {
                    path_type: qsTr("Shop Projects / Item Library")
                    setting: "shop"
                    width: parent.width
                }

                PathItem {
                    path_type: qsTr("Resources")
                    setting: "resources"
                    width: parent.width
                }

                Rectangle {
                    height: 10
                    width: height
                    color: "transparent"
                }
            }

            // Addons
            Column {
                spacing: 5

                Text {
                    id: addon_text
                    width: parent.width
                    text: qsTr("The following addons are available:")
                    color: color_scheme.textColor
                    font.pointSize: 12
                    clip: true
                    wrapMode: "WordWrap"
                }

                ScrollView {
                    width: parent.width
                    height: parent.height - addon_text.height - parent.spacing * 2
                    clip: true

                    Column {
                        id: addon_column
                        width: parent.parent.width
                        spacing: 10
                        topPadding: 10
                    }
                }
            }

            Column {
                id: help_column
                spacing: 5

                Button {
                    text: qsTr("Open Wiki")
                    width: parent.width > 400 ? parent.width / 3 : parent.width
                    anchors.horizontalCenter: parent.horizontalCenter

                    onClicked: Qt.openUrlExternally(
                                   "https://github.com/PhilInTheGaps/GM-Companion/wiki")
                }

                Button {
                    text: qsTr("Report a Bug")
                    width: parent.width > 400 ? parent.width / 3 : parent.width
                    anchors.horizontalCenter: parent.horizontalCenter

                    onClicked: Qt.openUrlExternally(
                                   "https://github.com/PhilInTheGaps/GM-Companion/issues")
                }

                Button {
                    text: qsTr("Download older Versions")
                    width: parent.width > 400 ? parent.width / 3 : parent.width
                    anchors.horizontalCenter: parent.horizontalCenter

                    onClicked: Qt.openUrlExternally(
                                   "https://github.com/PhilInTheGaps/GM-Companion/releases")
                }

                Button {
                    text: qsTr("View GM-Companion on GitHub")
                    width: parent.width > 400 ? parent.width / 3 : parent.width
                    anchors.horizontalCenter: parent.horizontalCenter

                    onClicked: Qt.openUrlExternally(
                                   "https://github.com/PhilInTheGaps/GM-Companion")
                }

                Button {
                    text: qsTr("Changelog")
                    width: parent.width > 400 ? parent.width / 3 : parent.width
                    anchors.horizontalCenter: parent.horizontalCenter

                    onClicked: Qt.openUrlExternally(
                                   "https://github.com/PhilInTheGaps/GM-Companion/wiki/changelog")
                }

                Button {
                    text: qsTr("About")
                    width: parent.width > 400 ? parent.width / 3 : parent.width
                    anchors.horizontalCenter: parent.horizontalCenter

                    onClicked: {
                        Qt.openUrlExternally(
                                    "https://gm-companion.github.io/about.html")
                    }
                }

                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Version: " + update_manager.getCurrentVersion()
                }
            }
        }

        Text {
            id: restart_info_text
            text: qsTr("Changing settings requires a program restart!")
            color: "red"
            padding: 5
        }
    }
}
