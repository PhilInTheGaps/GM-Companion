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
                text: FontAwesome.cloud
                font.family: FontAwesome.familySolid
            }

            Label {
                text: "NextCloud"
                font.bold: true
            }
        }

        // Login to new server
        Column {
            visible: !nextcloud_service.connected
            spacing: 10
            anchors.left: parent.left
            anchors.right: parent.right

            Grid {
                columns: 2
                spacing: 10
                anchors.left: parent.left
                anchors.right: parent.right
                verticalItemAlignment: Grid.AlignVCenter

                Label {
                    text: qsTr("Server URL")
                }

                CustomTextField {
                    id: server_textfield
                    selectByMouse: true
                    width: root.width / 2
                    placeholderText: "https://"
                    Component.onCompleted: text = settings_manager.getServerUrl(
                                               "NextCloud", false)
                }
            }

            Button {
                text: qsTr("Login")
                onClicked: {
                    settings_manager.setServerUrl(server_textfield.text,
                                                  "NextCloud")
                    nextcloud_service.connectService()
                }
            }
        }

        // Logged in
        Column {
            visible: nextcloud_service.connected
            spacing: 10
            anchors.left: parent.left
            anchors.right: parent.right

            Grid {
                columns: 2
                spacing: 10
                anchors.left: parent.left
                anchors.right: parent.right
                verticalItemAlignment: Grid.AlignVCenter

                Label {
                    text: qsTr("Login Name:")
                }

                Label {
                    text: nextcloud_service.loginName
                }

                Label {
                    text: qsTr("Server URL:")
                }

                Label {
                    text: nextcloud_service.serverUrl
                }
            }

            Button {
                text: qsTr("Logout")
                onClicked: nextcloud_service.disconnectService()
            }
        }
    }

    StatusBar {
        id: status_label
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        type: nextcloud_service.status.type
        text: nextcloud_service.status.message
    }
}
