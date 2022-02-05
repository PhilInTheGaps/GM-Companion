import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.1
import CustomComponents 1.0
import FontAwesome 2.0

BaseAccountPage {
    id: root

    name: "Discord"
    icon: FontAwesome.discord
    iconFamily: FontAwesome.familyBrands
    status: [discord_service.status]

    leftPanel: Column {
        spacing: 10

        CheckBox {
            id: use_discord_checkbox
            text: qsTr("Use Discord Bot")
            checked: discord_service.enabled

            onClicked: {
                settings_manager.setSetting("enabled", checked, "Discord")
                discord_service.enabled = checked
            }
        }

        GridLayout {
            columns: 2
            columnSpacing: 10
            rowSpacing: 10
            anchors.left: parent.left
            anchors.right: parent.right
            enabled: use_discord_checkbox.checked

            Label {
                text: qsTr("Channel ID")
                Layout.alignment: Qt.AlignVCenter
            }

            TextField {
                id: discord_channel_textfield
                Layout.fillWidth: true
                placeholderText: "xxxxxxxxxxxxxxxxxx"

                onTextEdited: {
                    settings_manager.setSetting("channel", text, "Discord")
                    discord_service.channel = text
                }

                Component.onCompleted: text = discord_service.channel
            }

            RadioButton {
                id: default_server_radio_button
                text: qsTr("Use default server")
                Layout.alignment: Qt.AlignVCenter

                checked: settings_manager.getSetting("connection", "default",
                                                     "Discord") === "default"

                onClicked: settings_manager.setSetting("connection", "default",
                                                       "Discord")
            }

            Item {
                Layout.fillWidth: true
            }

            RadioButton {
                id: custom_server_radio_button
                text: qsTr("Use custom server")
                Layout.alignment: Qt.AlignVCenter

                checked: settings_manager.getSetting("connection", "default",
                                                     "Discord") === "custom"

                onClicked: settings_manager.setSetting("connection", "custom",
                                                       "Discord")
            }

            TextField {
                id: custom_server_textfield
                Layout.fillWidth: true
                placeholderText: "https://"
                visible: custom_server_radio_button.checked

                onTextEdited: settings_manager.setServerUrl(text, "Discord")

                Component.onCompleted: text = settings_manager.getServerUrl(
                                           "Discord")
            }
        }

        Row {
            spacing: 10

            Button {
                text: qsTr("Check Connection")
                onClicked: {
                    discord_service.testConnection()
                }
            }

            Button {
                text: qsTr("Invite to Server")
                onClicked: {
                    discord_service.invite()
                }
            }
        }
    }
}
