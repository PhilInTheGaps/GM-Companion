import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Dialogs 1.3

import "./settings"

Page {
    id: settings_page

    Component.onCompleted: {
        addon_manager.updateAddonList()

        if (settings_tool.getCheckForUpdates())
            update_manager.checkForUpdates()
    }

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

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    FileDialog {
        id: file_dialog

        property var text_field

        selectFolder: true

        onAccepted: {
            if (platform.isWindows)
                text_field.text = fileUrl.toString().replace("file:///", "")
            else
                text_field.text = fileUrl.toString().replace("file://", "")
        }
    }

    header: TabBar {
        id: tab_bar

        height: platform.isAndroid ? width / 6 : color_scheme.toolbarHeight

        background: Rectangle {
            color: color_scheme.toolbarColor
        }

        Repeater {
            id: tab_button_repeater

            model: [qsTr("General"), qsTr("Paths"), qsTr("Cloud Storage"), qsTr(
                    "Addons"), qsTr("Info")]

            onItemAdded: tab_bar.currentIndex = 0

            TabButton {
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter

                Text {
                    text: modelData
                    color: color_scheme.toolbarTextColor
                    font.pointSize: 12
                    font.bold: true
                    anchors.centerIn: parent
                }

                Rectangle {
                    color: "white"
                    height: 2
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.margins: 5
                    visible: tab_bar.currentIndex === index
                }

                background: Rectangle {
                    color: color_scheme.toolbarColor
                }
            }
        }
    }

    footer: Rectangle {
        id: footer_bar
        height: color_scheme.toolbarHeight
        color: color_scheme.backgroundColor

        Label {
            anchors.centerIn: parent
            text: qsTr("Most settings require a program restart!")
            color: color_scheme.textColor
        }
    }

    contentItem: SwipeView {
        id: swipe_view
        anchors.left: parent.left
        anchors.right: parent.right
        currentIndex: tab_bar.currentIndex
        padding: 5
        spacing: 5
        interactive: false

        // General Settings
        GeneralPage {
            id: general_page
        }

        // Paths
        PathsPage {
            id: paths_page
        }

        // Cloud Storage
        CloudPage {
            id: cloud_page
        }

        // Addons
        AddonsPage {
            id: addons_page
        }

        // Info
        InfoPage {
            id: info_page
        }
    }
}
