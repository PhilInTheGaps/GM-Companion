import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import Qt.labs.platform 1.0

import gm.companion.platforms 1.0
import gm.companion.settingstool 1.0
import gm.companion.updatemanager 1.0
import gm.companion.colorscheme 1.0
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

    PlatformDetails {
        id: platform_details
    }

    ColorScheme {
        id: color_scheme
    }

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    ScrollView {
        id: settings_scroll_view
        width: parent.width
        height: parent.height
        clip: true

        Flow {
            id: settings_flow
            width: settings_scroll_view.width
            padding: 5
            spacing: 5

            Column {
                id: settings_column
                width: settings_page.width
                       > settings_page.height ? settings_flow.width / 2 - parent.spacing
                                                - parent.padding : settings_flow.width
                                                - parent.padding * 2

                spacing: 5

                Text {
                    text: qsTr("Changing settings requires a program restart!")
                    color: color_scheme.textColor
                }

                Text {
                    text: qsTr("Language")
                    font.bold: true
                    color: color_scheme.textColor
                }

                Row {
                    width: parent.width

                    Text {
                        text: qsTr("Select Language")
                        width: parent.width / 2
                        anchors.verticalCenter: parent.verticalCenter
                        color: color_scheme.textColor
                    }

                    ComboBox {
                        property bool loaded: false
                        width: parent.width / 2
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
                        width: parent.width / 2
                        anchors.verticalCenter: parent.verticalCenter
                        color: color_scheme.textColor
                    }

                    ComboBox {
                        property bool loaded: false
                        width: parent.width / 2
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

                Rectangle {
                    height: 10
                    width: height
                    color: "transparent"
                }

                Text {
                    text: qsTr("Paths")
                    font.bold: true
                    color: color_scheme.textColor
                }

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
                    path_type: qsTr("Shop Projects / Item Database")
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

                Text {
                    text: qsTr("Addons")
                    font.bold: true
                    color: color_scheme.textColor
                }

                Text {
                    text: "// TODO"
                    color: color_scheme.textColor
                }
            }

            Column {
                id: help_column
                width: settings_page.width
                       > settings_page.height ? settings_flow.width / 2 - parent.spacing
                                                - parent.padding : settings_flow.width
                                                - parent.padding * 2
                spacing: 5

                Text {
                    text: qsTr("Help")
                    font.bold: true
                    color: color_scheme.textColor
                }

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
    }
}
