import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.3

//import Qt.labs.platform 1.0
import gm.companion.settingstool 1.0
import gm.companion.platforms 1.0

Column {
    id: column
    property var path_type
    property var setting

    SettingsTool {
        id: settings
    }

    PlatformDetails {
        id: platform_details
    }

    Text {
        text: path_type
        color: color_scheme.textColor
    }

    Row {
        spacing: 5
        width: parent.width

        TextField {
            id: path_text_field
            width: button.visible ? parent.width - button.width - parent.spacing : parent.width
            text: settings.getPath(setting)
            selectByMouse: true

            onTextChanged: {
                settings.setPath(setting, text)
            }
        }

        Button {
            id: button
            text: "..."

            onClicked: {
                file_dialog.open()
            }
        }
    }

    FileDialog {
        id: file_dialog
        title: "Set " + path_type + " Folder"

        folder: "file://" + path_text_field.text

        selectFolder: true

        onAccepted: path_text_field.text = fileUrl.toString().replace(
                        "file://", "")
    }
}
