import QtQuick 2.9
import QtQuick.Controls 2.2
import "../common"

Dialog {
    id: root

    title: qsTr("New Settings")

    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    modal: true
    standardButtons: Dialog.Close

    Column {
        spacing: 10

        Label {
            text: qsTr("Telemetry")
            font.bold: true
        }

        // Crash Reports
        Row {
            CheckBox {
                id: crash_reports_box
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
                id: session_tracking_box
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

    onClosed: {
        settings_manager.setSetting("crashReports",
                                    crash_reports_box.checked ? 1 : 0,
                                    "Telemetry")
        settings_manager.setSetting("sessionTracking",
                                    session_tracking_box.checked ? 1 : 0,
                                    "Telemetry")
    }
}
