import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0

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

            CustomComboBox {
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
        CustomCheckBox {
            text: qsTr("Show tool names in menu")
            checked: settings_manager.getBoolSetting("showToolNames", false)
            onClicked: settings_manager.showToolNames = checked
        }

        // Classic Icons
        CustomCheckBox {
            text: qsTr("Classic menu icons")
            checked: settings_manager.getBoolSetting("classicIcons", false)
            onClicked: settings_manager.classicIcons = checked
        }
    }

    // Updates
    Connections {
        target: update_manager

        onUpdateAvailable: {
            update_text.text = qsTr(
                        "Found new Version: ") + update_manager.newestVersion
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

    Column {
        spacing: 10

        Label {
            text: qsTr("Updates")
            font.bold: true
            visible: platform.isAndroid ? false : true
        }

        CustomCheckBox {
            text: qsTr("Automatically check for updates")
            checked: settings_manager.getBoolSetting("checkForUpdates", false,
                                                     "Updates")

            onCheckedChanged: settings_manager.setSetting("checkForUpdates",
                                                          checked.toString(),
                                                          "Updates")
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
