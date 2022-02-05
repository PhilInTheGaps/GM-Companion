import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.1
import CustomComponents 1.0
import FontAwesome 2.0

BaseAccountPage {
    id: root

    name: "NextCloud"
    icon: FontAwesome.cloud
    iconFamily: FontAwesome.familySolid
    status: [nextcloud_service.status]

    leftPanel: Column {
        spacing: 10

        // Login to new server
        Column {
            visible: !nextcloud_service.connected
            spacing: 10
            anchors.left: parent.left
            anchors.right: parent.right

            GridLayout {
                columns: 2
                columnSpacing: 10
                anchors.left: parent.left
                anchors.right: parent.right

                Label {
                    text: qsTr("Server URL")
                    Layout.alignment: Qt.AlignVCenter
                }

                TextField {
                    id: server_textfield
                    selectByMouse: true
                    Layout.fillWidth: true
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

            GridLayout {
                columns: 2
                columnSpacing: 10
                rowSpacing: 10
                anchors.left: parent.left
                anchors.right: parent.right

                Label {
                    text: qsTr("Login Name:")
                    Layout.alignment: Qt.AlignTop
                }

                Label {
                    text: nextcloud_service.loginName
                    Layout.alignment: Qt.AlignTop
                    Layout.fillWidth: true
                    wrapMode: Label.WrapAnywhere
                }

                Label {
                    text: qsTr("Server URL:")
                    Layout.alignment: Qt.AlignTop
                }

                Label {
                    text: nextcloud_service.serverUrl
                    Layout.alignment: Qt.AlignTop
                    Layout.fillWidth: true
                    wrapMode: Label.WrapAnywhere
                }
            }

            Button {
                text: qsTr("Logout")
                onClicked: nextcloud_service.disconnectService()
            }
        }
    }
}
