import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import "../../common"

Column {
    id: general_column
    spacing: 20
    padding: 10

    // Language
    Column {
        spacing: 10

        Label {
            text: qsTr("Language")
            font.bold: true
        }

        Row {
            spacing: 10

            Label {
                text: qsTr("Select Language")
                width: platform.isAndroid ? settings_page.width / 4 : language_box.width
                anchors.verticalCenter: parent.verticalCenter
            }

            ComboBox {
                id: language_box

                property bool loaded: false
                property int originalIndex: -1

                model: settings_manager.getLanguageNames()
                width: settings_page.width / 6

                onCurrentTextChanged: {
                    if (loaded) {
                        settings_manager.setLanguage(currentText)
                    }
                }

                Component.onCompleted: {
                    currentIndex = settings_manager.getLanguageIndex()
                    originalIndex = currentIndex
                    loaded = true
                }
            }

            Label {
                id: requires_restart_label
                visible: language_box.currentIndex !== language_box.originalIndex
                text: qsTr("Restart required!")
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    // UI
    Column {
        spacing: 10

        Label {
            text: qsTr("User Interface")
            font.bold: true
        }

        // Show Tool Names
        CheckBox {
            text: qsTr("Show tool names in menu")
            checked: settings_manager.getBoolSetting("showToolNames", false)
            onClicked: settings_manager.showToolNames = checked
        }

        // Classic Icons
        CheckBox {
            text: qsTr("Classic menu icons")
            checked: settings_manager.getBoolSetting("classicIcons", false)
            onClicked: settings_manager.classicIcons = checked
        }
    }

    // Telemetry
    Column {
        spacing: 10

        Label {
            text: qsTr("Telemetry")
            font.bold: true
        }

        // Crash Reports
        Row {
            CheckBox {
                text: qsTr("Automated crash reports")
                checked: settings_manager.getBoolSetting("crashReports", false,
                                                         "Telemetry")
                onClicked: settings_manager.setSetting("crashReports",
                                                       checked ? 1 : 0,
                                                       "Telemetry")
            }

            HelpAnnotation {
                anchors.verticalCenter: parent.verticalCenter
                helpText: qsTr("When the application crashes, a report will be uploaded to sentry.io \nso that the issue can be fixed in a future release.")
            }
        }

        // Session Tracking
        Row {
            CheckBox {
                text: qsTr("Session tracking")
                checked: settings_manager.getBoolSetting("sessionTracking",
                                                         false, "Telemetry")
                onClicked: settings_manager.setSetting("sessionTracking",
                                                       checked ? 1 : 0,
                                                       "Telemetry")
            }

            HelpAnnotation {
                anchors.verticalCenter: parent.verticalCenter
                helpText: qsTr("The application will keep track of how long it is used and if any errors occured. \nIt will then send an anonymous report to sentry.io so that we know how stable the application is.")
            }
        }
    }

    // Updates
    Connections {
        target: update_manager

        function onUpdateAvailable() {
            update_text.text = qsTr(
                        "Found new Version: ") + update_manager.newestVersion
            update_text.visible = true
            open_downloads_button.visible = true
            update_busy_indicator.visible = false
        }

        function onNoUpdateAvailable() {
            update_text.text = qsTr("No newer version found")
            update_text.visible = true
            update_busy_indicator.visible = false
        }
    }

    Column {
        spacing: 10

        Label {
            text: qsTr("Updates")
            font.bold: true
            visible: platform.isAndroid ? false : true
        }

        Row {
            CheckBox {
                text: qsTr("Automatically check for updates")
                checked: settings_manager.getBoolSetting("checkForUpdates",
                                                         false, "Updates")

                onCheckedChanged: settings_manager.setSetting(
                                      "checkForUpdates", checked.toString(),
                                      "Updates")
            }

            HelpAnnotation {
                anchors.verticalCenter: parent.verticalCenter
                helpText: qsTr("If enabled, the application will automatically check if there are any updates available and notify you if that is the case.\nThis is disabled by default if you installed the application through a package manager.")
            }
        }

        Row {
            spacing: 5

            Button {
                id: check_for_updates_button
                text: qsTr("Check for Updates")
                visible: platform.isAndroid ? false : true

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

            Label {
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
}
