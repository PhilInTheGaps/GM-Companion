import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import IconFonts
import common as Common
import services as Services

BaseAccountPage {
    id: root

    name: "NextCloud"
    icon: FontAwesome.cloud
    iconFont: FontAwesome.fontSolid
    statuses: [Services.NextCloud.status]

    leftPanel: Column {
        spacing: 10

        // Login to new server
        Column {
            visible: !Services.NextCloud.connected
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
                    Component.onCompleted: text = Common.SettingsManager.services.nextcloud.getServerUrl()
                }
            }

            Button {
                text: qsTr("Login")
                onClicked: {
                    Common.SettingsManager.services.nextcloud.setServerUrl(server_textfield.text);
                    Services.NextCloud.connectService();
                }
            }
        }

        // Logged in
        Column {
            visible: Services.NextCloud.connected
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
                    text: Services.NextCloud.loginName
                    Layout.alignment: Qt.AlignTop
                    Layout.fillWidth: true
                    wrapMode: Label.WrapAnywhere
                }

                Label {
                    text: qsTr("Server URL:")
                    Layout.alignment: Qt.AlignTop
                }

                Label {
                    text: Services.NextCloud.serverUrl
                    Layout.alignment: Qt.AlignTop
                    Layout.fillWidth: true
                    wrapMode: Label.WrapAnywhere
                }
            }

            Button {
                text: qsTr("Logout")
                onClicked: Services.NextCloud.disconnectService()
            }
        }
    }
}
