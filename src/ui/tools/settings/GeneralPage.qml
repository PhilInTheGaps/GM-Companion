import QtQuick 2.9
import QtQuick.Controls 2.2

Column {
    id: general_column
    spacing: 20

    // Language
    Column {
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 5

        Text {
            text: qsTr("Language")
            font.bold: true
            color: color_scheme.textColor
        }

        Row {
            anchors.left: parent.left
            anchors.right: parent.right

            Text {
                text: qsTr("Select Language")
                width: platform.isAndroid ? settings_page.width / 4 : language_box.width
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
    }

    // UI
    Column {
        spacing: 5
        anchors.left: parent.left
        anchors.right: parent.right

        Text {
            text: qsTr("User Interface")
            font.bold: true
            color: color_scheme.textColor
        }

        // UI Style
        Row {
            anchors.left: parent.left
            anchors.right: parent.right

            Text {
                text: qsTr("Select Style")
                width: platform.isAndroid ? settings_page.width / 4 : language_box.width
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
                        color_scheme.updateColors()
                    }
                }

                Component.onCompleted: {
                    currentIndex = settings_tool.uiStyleIndex
                    loaded = true
                }
            }
        }

        // Show Tool Names
        Row {
            CheckBox {
                checked: settings_tool.showToolNames

                onCheckedChanged: {
                    settings_tool.showToolNames = checked
                }
            }

            Text {
                text: qsTr("Show tool names in menu")
                color: color_scheme.textColor
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        // Classic Icons
        Row {
            CheckBox {
                checked: settings_tool.classicIcons

                onCheckedChanged: {
                    settings_tool.classicIcons = checked
                }
            }

            Text {
                text: qsTr("Classic menu icons")
                color: color_scheme.textColor
                anchors.verticalCenter: parent.verticalCenter
            }
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
        spacing: 5
        anchors.left: parent.left
        anchors.right: parent.right

        Text {
            text: qsTr("Updates")
            font.bold: true
            visible: platform.isAndroid ? false : true
            color: color_scheme.textColor
        }

        Row {
            anchors.left: parent.left
            anchors.right: parent.right
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
    }
}
