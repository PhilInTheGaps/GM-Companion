import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.1
import CustomComponents 1.0
import FontAwesome 2.0

BaseAccountPage {
    id: root

    name: "GoogleDrive"
    icon: FontAwesome.googleDrive
    iconFamily: FontAwesome.familyBrands
    status: [googledrive_service.status]

    leftPanel: Column {
        spacing: 10

        // Connected
        GridLayout {
            visible: googledrive_service.connected
            columns: 2
            columnSpacing: 10
            anchors.left: parent.left
            anchors.right: parent.right

            Label {
                text: qsTr("Client ID:")
                Layout.alignment: Qt.AlignTop
            }

            Label {
                Component.onCompleted: text = googledrive_service.clientId
                wrapMode: Label.WrapAnywhere
                Layout.fillWidth: true
            }
        }

        // Not connected
        GridLayout {
            visible: !googledrive_service.connected
            columns: 2
            columnSpacing: 10
            anchors.left: parent.left
            anchors.right: parent.right

            Label {
                text: qsTr("Client ID")
                Layout.alignment: Qt.AlignVCenter
            }

            TextField {
                id: google_id_textfield
                selectByMouse: true
                Layout.fillWidth: true
                Component.onCompleted: text = settings_manager.googleID
            }

            Label {
                text: qsTr("Client Secret")
                Layout.alignment: Qt.AlignVCenter
            }

            TextField {
                id: google_secret_textfield
                selectByMouse: true
                Layout.fillWidth: true
                Component.onCompleted: text = settings_manager.googleSecret
            }
        }

        Button {
            text: googledrive_service.connected ? qsTr("Disconnect") : qsTr(
                                                      "Connect")
            onClicked: {
                if (googledrive_service.connected) {
                    googledrive_service.disconnectService()
                } else {
                    settings_manager.googleID = google_id_textfield.text
                    settings_manager.googleSecret = google_secret_textfield.text
                    google_secret_textfield.text = ""
                    googledrive_service.connectService()
                }
            }
        }
    }
}
