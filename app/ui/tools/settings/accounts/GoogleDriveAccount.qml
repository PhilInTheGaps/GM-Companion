import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import IconFonts
import common as Common

//import services

BaseAccountPage {
    id: root

    name: "GoogleDrive"
    icon: FontAwesome.googleDrive
    iconFont: FontAwesome.fontBrands

    //    statuses: [GoogleDrive.status]

    leftPanel: Column {
        spacing: 10

        // Connected
        GridLayout {
            //            visible: GoogleDrive.connected
            columns: 2
            columnSpacing: 10
            anchors.left: parent.left
            anchors.right: parent.right

            Label {
                text: qsTr("Client ID:")
                Layout.alignment: Qt.AlignTop
            }

            Label {
                //                Component.onCompleted: text = GoogleDrive.clientId
                wrapMode: Label.WrapAnywhere
                Layout.fillWidth: true
            }
        }

        // Not connected
        GridLayout {
            //            visible: !GoogleDrive.connected
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
                Component.onCompleted: text = Common.SettingsManager.services.google.clientId
            }

            Label {
                text: qsTr("Client Secret")
                Layout.alignment: Qt.AlignVCenter
            }

            TextField {
                id: google_secret_textfield
                selectByMouse: true
                Layout.fillWidth: true
                Component.onCompleted: text = Common.SettingsManager.services.google.clientSecret
            }
        }

        //            text: GoogleDrive.connected ? qsTr("Disconnect") : qsTr("Connect")
        //            onClicked: {
        //                if (GoogleDrive.connected) {
        //                    GoogleDrive.disconnectService()
        //                } else {
        //                    SettingsManager.googleID = google_id_textfield.text
        //                    SettingsManager.googleSecret = google_secret_textfield.text
        //                    google_secret_textfield.text = ""
        //                    GoogleDrive.connectService()
        //                }
        //            }
        Button {
        }
    }
}
