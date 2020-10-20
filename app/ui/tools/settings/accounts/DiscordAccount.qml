import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0

Item {
    id: root
    anchors.left: parent.left
    anchors.right: parent.right

    Column {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: status_label.visible ? status_label.top : parent.bottom
        spacing: 10

        Row {
            spacing: 5

            Label {
                text: FontAwesome.discord
                font.family: FontAwesome.familyBrands
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
            }

            Label {
                text: "Discord"
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        CheckBox {
            id: use_discord_checkbox
            text: qsTr("Use Discord Bot")
            checked: discord_service.enabled

            onClicked: {
                settings_manager.setSetting("enabled", checked, "Discord")
                discord_service.enabled = checked
            }
        }

        Grid {
            columns: 2
            spacing: 10
            anchors.left: parent.left
            anchors.right: parent.right
            verticalItemAlignment: Grid.AlignVCenter
            enabled: use_discord_checkbox.checked

            Label {
                text: qsTr("Channel ID")
            }

            TextField {
                id: discord_channel_textfield
                width: root.width / 2
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
                checked: settings_manager.getSetting("connection", "default",
                                                     "Discord") === "default"

                onClicked: settings_manager.setSetting("connection", "default",
                                                       "Discord")
            }

            Item {
                width: 5
                height: 5
            }

            RadioButton {
                id: custom_server_radio_button
                text: qsTr("Use custom server")
                checked: settings_manager.getSetting("connection", "default",
                                                     "Discord") === "custom"

                onClicked: settings_manager.setSetting("connection", "custom",
                                                       "Discord")
            }

            TextField {
                id: custom_server_textfield
                width: root.width / 2
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

    StatusBar {
        id: status_label
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        type: discord_service.status.type
        text: discord_service.status.message
    }
}
