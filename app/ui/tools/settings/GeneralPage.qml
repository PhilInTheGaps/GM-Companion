import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import CustomComponents
import IconFonts
import common
import "../../common"

SplitView {
    id: root

    handle: Item {
    }

    ColorDialog {
        id: color_dialog
        title: qsTr("Choose a color")
    }

    Column {
        id: left_column
        spacing: 20
        padding: 10

        // UI
        Column {
            spacing: 10

            Label {
                text: qsTr("User Interface")
                font.bold: true
            }

            Row {
                spacing: 10

                Label {
                    text: qsTr("Select Language")
                    width: Math.max(implicitWidth, language_box.width)
                    anchors.verticalCenter: parent.verticalCenter
                }

                ComboBox {
                    id: language_box

                    property bool loaded: false

                    model: SettingsManager.getLanguageNames()

                    onCurrentTextChanged: {
                        if (loaded) {
                            SettingsManager.setLanguage(currentText);
                        }
                    }

                    Component.onCompleted: {
                        currentIndex = SettingsManager.getLanguageIndex();
                        loaded = true;
                    }
                }
            }

            // Show Tool Names
            CheckBox {
                text: qsTr("Show tool names in menu")
                checked: SettingsManager.window.showToolNames
                onClicked: SettingsManager.window.showToolNames = checked
            }

            Row {
                spacing: 10

                CustomColorButton {
                    color: SettingsManager.colors.info
                    enableReset: true

                    property bool active: false

                    function saveColor() {
                        if (!active)
                            return;
                        disconnect();
                        SettingsManager.colors.info = color_dialog.selectedColor;
                    }

                    function disconnect() {
                        color_dialog.accepted.disconnect(saveColor);
                        color_dialog.rejected.disconnect(disconnect);
                        active = false;
                    }

                    onClicked: {
                        active = true;
                        color_dialog.accepted.connect(saveColor);
                        color_dialog.rejected.connect(disconnect);
                        color_dialog.selectedColor = SettingsManager.colors.info;
                        color_dialog.open();
                    }

                    onResetClicked: SettingsManager.colors.resetInfo()
                }

                Label {
                    font.family: FontAwesome.fontSolid.family
                    font.styleName: FontAwesome.fontSolid.styleName
                    font.pointSize: 12
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter

                    color: SettingsManager.colors.info
                    text: FontAwesome.circleInfo
                }

                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("Info")
                }
            }

            Row {
                spacing: 10

                CustomColorButton {
                    color: SettingsManager.colors.success
                    enableReset: true

                    property bool active: false

                    function saveColor() {
                        if (!active)
                            return;
                        disconnect();
                        SettingsManager.colors.success = color_dialog.selectedColor;
                    }

                    function disconnect() {
                        color_dialog.accepted.disconnect(saveColor);
                        color_dialog.rejected.disconnect(disconnect);
                        active = false;
                    }

                    onClicked: {
                        active = true;
                        color_dialog.accepted.connect(saveColor);
                        color_dialog.rejected.connect(disconnect);
                        color_dialog.selectedColor = SettingsManager.colors.success;
                        color_dialog.open();
                    }

                    onResetClicked: SettingsManager.colors.resetSuccess()
                }

                Label {
                    font.family: FontAwesome.fontSolid.family
                    font.styleName: FontAwesome.fontSolid.styleName
                    font.pointSize: 12
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter

                    color: SettingsManager.colors.success
                    text: FontAwesome.circleCheck
                }

                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("Success")
                }
            }

            Row {
                spacing: 10

                CustomColorButton {
                    color: SettingsManager.colors.warning
                    enableReset: true

                    property bool active: false

                    function saveColor() {
                        if (!active)
                            return;
                        disconnect();
                        SettingsManager.colors.warning = color_dialog.selectedColor;
                    }

                    function disconnect() {
                        color_dialog.accepted.disconnect(saveColor);
                        color_dialog.rejected.disconnect(disconnect);
                        active = false;
                    }

                    onClicked: {
                        active = true;
                        color_dialog.accepted.connect(saveColor);
                        color_dialog.rejected.connect(disconnect);
                        color_dialog.selectedColor = SettingsManager.colors.warning;
                        color_dialog.open();
                    }

                    onResetClicked: SettingsManager.colors.resetWarning()
                }

                Label {
                    font.family: FontAwesome.fontSolid.family
                    font.styleName: FontAwesome.fontSolid.styleName
                    font.pointSize: 12
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter

                    color: SettingsManager.colors.warning
                    text: FontAwesome.triangleExclamation
                }

                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("Warning")
                }
            }

            Row {
                spacing: 10

                CustomColorButton {
                    color: SettingsManager.colors.error
                    enableReset: true

                    property bool active: false

                    function saveColor() {
                        if (!active)
                            return;
                        disconnect();
                        SettingsManager.colors.error = color_dialog.selectedColor;
                    }

                    function disconnect() {
                        color_dialog.accepted.disconnect(saveColor);
                        color_dialog.rejected.disconnect(disconnect);
                        active = false;
                    }

                    onClicked: {
                        active = true;
                        color_dialog.accepted.connect(saveColor);
                        color_dialog.rejected.connect(disconnect);
                        color_dialog.selectedColor = SettingsManager.colors.error;
                        color_dialog.open();
                    }

                    onResetClicked: SettingsManager.colors.resetError()
                }

                Label {
                    font.family: FontAwesome.fontSolid.family
                    font.styleName: FontAwesome.fontSolid.styleName
                    font.pointSize: 12
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter

                    color: SettingsManager.colors.error
                    text: FontAwesome.circleExclamation
                }

                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("Error")
                }
            }
        }
    }

    Column {
        id: right_column
        spacing: 20
        padding: 10

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
                update_text.text = qsTr("Found new Version: ") + UpdateManager.newestVersion;
                update_text.visible = true;
                update_icon.text = FontAwesome.circleInfo;
                update_icon.color = SettingsManager.colors.info;
                open_downloads_button.visible = true;
                update_busy_indicator.visible = false;
            }

            function onNoUpdateAvailable() {
                update_text.text = qsTr("No newer version found");
                update_text.visible = true;
                update_icon.text = FontAwesome.circleCheck;
                update_icon.color = SettingsManager.colors.success;
                update_busy_indicator.visible = false;
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
                        update_text.visible = false;
                        update_busy_indicator.visible = true;
                        UpdateManager.checkForUpdates();
                    }
                }

                BusyIndicator {
                    id: update_busy_indicator
                    visible: false
                    width: check_for_updates_button.height
                    height: width
                }

                Label {
                    id: update_icon
                    visible: update_text.visible

                    font.family: FontAwesome.fontSolid.family
                    font.styleName: FontAwesome.fontSolid.styleName
                    font.pointSize: 12
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter

                    text: FontAwesome.circleCheck
                    color: SettingsManager.colors.success
                }

                Label {
                    id: update_text
                    anchors.verticalCenter: parent.verticalCenter
                    visible: false
                }
            }

            Button {
                id: open_downloads_button
                visible: false
                text: qsTr("Open Download Page")

                onClicked: {
                    Qt.openUrlExternally("https://github.com/PhilInTheGaps/GM-Companion/releases");
                }
            }
        }
    }
}
