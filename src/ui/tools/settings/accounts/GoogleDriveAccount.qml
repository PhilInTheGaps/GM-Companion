import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0

Column {
    id: root

    anchors.left: parent.left
    anchors.right: parent.right
    spacing: 10

    Row {
        spacing: 5

        Label {
            text: FontAwesome.googleDrive
            font.family: FontAwesome.familyBrands
        }

        Label {
            text: "GoogleDrive"
            font.bold: true
        }
    }

    Grid {
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
        text: qsTr("Save and Connect")
        onClicked: {
            settings_manager.setSetting("googleID",
                                        google_id_textfield.text, "Google")
            settings_manager.setSetting("googleSecret",
                                        google_secret_textfield.text, "Google")

            googledrive_service.updateConnector()
        }
    }

    Label {
        id: status_label

        font.pointSize: 12
        visible: text !== ""
        padding: 5

        text: googledrive_service.status
    }
}
