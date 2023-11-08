import QtQuick
import QtQuick.Controls
import common
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
                width: language_box.width
                anchors.verticalCenter: parent.verticalCenter
            }

            ComboBox {
                id: language_box

                property bool loaded: false

                model: SettingsManager.getLanguageNames()
                width: general_column.width / 6

                onCurrentTextChanged: {
                    if (loaded) {
                        SettingsManager.setLanguage(currentText)
                    }
                }

                Component.onCompleted: {
                    currentIndex = SettingsManager.getLanguageIndex()
                    loaded = true
                }
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
            checked: SettingsManager.showToolNames
            onClicked: SettingsManager.showToolNames = checked
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
                checked: SettingsManager.crashReports
                onClicked: SettingsManager.crashReports = checked
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
                checked: SettingsManager.sessionTracking
                onClicked: SettingsManager.sessionTracking = checked
            }

            HelpAnnotation {
                anchors.verticalCenter: parent.verticalCenter
                helpText: qsTr("The application will keep track of how long it is used and if any errors occured. \nIt will then send an anonymous report to sentry.io so that we know how stable the application is.")
            }
        }
    }

    // Updates
    Connections {
        target: UpdateManager

        function onUpdateAvailable() {
            update_text.text = qsTr(
                        "Found new Version: ") + UpdateManager.newestVersion
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
        }

        Row {
            CheckBox {
                text: qsTr("Automatically check for updates")
                checked: SettingsManager.checkForUpdates
                onClicked: SettingsManager.checkForUpdates = checked
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

                onClicked: {
                    update_text.visible = false
                    update_busy_indicator.visible = true
                    UpdateManager.checkForUpdates()
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
