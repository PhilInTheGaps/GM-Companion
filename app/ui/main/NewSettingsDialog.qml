import QtQuick
import QtQuick.Controls
import common
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
                id: session_tracking_box
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

    onClosed: {
        SettingsManager.crashReports = crash_reports_box.checked
        SettingsManager.sessionTracking = session_tracking_box.checked
    }
}
