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
                text: FontAwesome.googleDrive
                font.family: FontAwesome.familyBrands
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
            }

            Label {
                text: "GoogleDrive"
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        Grid {
            visible: googledrive_service.connected
            columns: 2
            spacing: 10
            anchors.left: parent.left
            anchors.right: parent.right
            verticalItemAlignment: Grid.AlignVCenter

            Label {
                text: qsTr("Client ID")
            }

            Label {
                Component.onCompleted: text = googledrive_service.clientId
            }
        }

        Grid {
            visible: !googledrive_service.connected
            columns: 2
            spacing: 10
            anchors.left: parent.left
            anchors.right: parent.right
            verticalItemAlignment: Grid.AlignVCenter

            Label {
                text: qsTr("Client ID")
            }

            CustomTextField {
                id: google_id_textfield
                selectByMouse: true
                width: root.width / 2
                Component.onCompleted: text = settings_manager.getSetting(
                                           "googleID", "", "Google")
            }

            Label {
                text: qsTr("Client Secret")
            }

            CustomTextField {
                id: google_secret_textfield
                selectByMouse: true
                width: root.width / 2
                Component.onCompleted: text = settings_manager.getSetting(
                                           "googleSecret", "", "Google")
            }
        }

        Button {
            text: googledrive_service.connected ? qsTr("Disconnect") : qsTr("Connect")
            onClicked: {
                if (googledrive_service.connected) {
                    googledrive_service.disconnectService()
                } else {
                    settings_manager.setSetting("googleID",
                                                google_id_textfield.text, "Google")
                    settings_manager.setSetting("googleSecret",
                                                google_secret_textfield.text,
                                                "Google")
                    google_secret_textfield.text = ""
                    googledrive_service.connectService()
                }
            }
        }
    }

    StatusBar {
        id: status_label
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        type: googledrive_service.status.type
        text: googledrive_service.status.message
    }
}
