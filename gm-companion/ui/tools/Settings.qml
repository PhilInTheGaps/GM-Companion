import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import Qt.labs.platform 1.0

import gm.companion.platforms 1.0
import gm.companion.settingstool 1.0
import gm.companion.updatemanager 1.0
import gm.companion.colorscheme 1.0
import gm.companion.addonmanager 1.0
import com.blackgrain.qml.quickdownload 1.0
import "./settings"

Page {
    id: settings_page

    SettingsTool {
        id: settings_tool
    }

    UpdateManager {
        id: update_manager
        Component.onCompleted: {
            setCurrentVersion(1000)
            checkForUpdates()

            if (getCurrentVersion() > newestVersionInt())
                addon_manager.setIsTestVersion(true)

            addon_manager.updateAddonList()
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
            addon_busy_indicator.visible = false
            addon_column.children = []

            var component = Qt.createComponent("./settings/AddonItem.qml")

            for (var i = 0; i < getAddonNames().length; i++) {
                var installed = false
                var outdated = false

                if (getAddonStatusList()[i] !== 0) {
                    installed = true

                    if (getAddonStatusList()[i] === 1) {
                        outdated = true
                    }
                }

                var addonItem = component.createObject(addon_column, {
                                                           x: 0,
                                                           y: 0,
                                                           addon: getAddonNames(
                                                                      )[i],
                                                           version: getAddonVersions(
                                                                        )[i],
                                                           description: getAddonDescriptions(
                                                                            )[i],
                                                           installed: installed,
                                                           outdated: outdated,
                                                           destination: getDestinationFolder()
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

    Column {
        anchors.fill: parent
        bottomPadding: 5

        TabBar {
            id: tab_bar
            width: parent.width - parent.padding * 2

            TabButton {
                text: qsTr("General")
            }

            TabButton {
                text: qsTr("Paths")
            }

            TabButton {
                text: qsTr("Addons")
            }

            TabButton {
                text: qsTr("Info")
            }
        }

        SwipeView {
            id: swipe_view
            width: parent.width - parent.padding * 2
            height: parent.height - parent.spacing * 2 - parent.bottomPadding
                    - tab_bar.height - restart_info_text.height
            currentIndex: tab_bar.currentIndex
            padding: 5
            spacing: 5
            interactive: false

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
                        width: language_box.width
                        anchors.verticalCenter: parent.verticalCenter
                        color: color_scheme.textColor
                    }

                    ComboBox {
                        id: language_box
                        property bool loaded: false
                        model: ["English", "Deutsch"]

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

                Row {
                    width: parent.width

                    Text {
                        text: qsTr("Select Style")
                        width: style_box.width
                        anchors.verticalCenter: parent.verticalCenter
                        color: color_scheme.textColor
                    }

                    ComboBox {
                        id: style_box
                        property bool loaded: false
                        model: ["Dark", "Bright"]

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

                Button {
                    id: open_downloads_button
                    visible: false
                    text: qsTr("Open Download Page")

                    onClicked: {
                        Qt.openUrlExternally(
                                    "https://github.com/PhilInTheGaps/GM-Companion/releases")
                    }
                }
            }

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

            Column {
                spacing: 5

                TextArea {
                    width: parent.width
                    padding: 0
                    readOnly: true

                    text: qsTr("Addons can be installed by clicking \"Download\". A download starts automatically and when it is finished, the downloaded archive is opened. \nTo finish the installation extract the content (A folder with the name of the addon) to the directory it was downloaded into. (your/home/folder/.gm-companion/addons) \nAfter installing addons the program must be restarted for them to work!")
                    clip: true
                    wrapMode: "WordWrap"
                }

                BusyIndicator {
                    id: addon_busy_indicator
                    visible: true
                    width: check_for_updates_button.height
                    height: width
                }

                Column {
                    id: addon_column
                    width: parent.width
                }
            }

            Column {
                id: help_column
                spacing: 5

                Button {
                    text: qsTr("Open Wiki")
                    width: parent.width > 400 ? 400 : parent.width

                    onClicked: Qt.openUrlExternally(
                                   "https://github.com/PhilInTheGaps/GM-Companion/wiki")
                }

                Button {
                    text: qsTr("Report a Bug")
                    width: parent.width > 400 ? 400 : parent.width

                    onClicked: Qt.openUrlExternally(
                                   "https://github.com/PhilInTheGaps/GM-Companion/issues")
                }

                Button {
                    text: qsTr("Download older Versions")
                    width: parent.width > 400 ? 400 : parent.width

                    onClicked: Qt.openUrlExternally(
                                   "https://github.com/PhilInTheGaps/GM-Companion/releases")
                }

                Button {
                    text: qsTr("View GM-Companion on GitHub")
                    width: parent.width > 400 ? 400 : parent.width

                    onClicked: Qt.openUrlExternally(
                                   "https://github.com/PhilInTheGaps/GM-Companion")
                }

                Button {
                    text: qsTr("About")
                    width: parent.width > 400 ? 400 : parent.width

                    onClicked: {
                        Qt.openUrlExternally(
                                    "https://gm-companion.github.io/pages/about/")
                    }
                }
            }
        }

        Text {
            id: restart_info_text
            text: qsTr("Changing settings requires a program restart!")
            color: "red"
        }
    }
}
