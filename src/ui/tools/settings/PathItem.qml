import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2

Column {
    id: column
    property string path_type
    property string setting
    spacing: 5

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
            text: settings_tool.getPath(setting)
            selectByMouse: true

            onTextChanged: {
                settings_tool.setPath(setting, text)
            }
        }

        Button {
            id: button
            text: "..."

            onClicked: {
                file_dialog.title = "Set " + path_type + " Folder"
                file_dialog.folder = "file://" + path_text_field.text
                file_dialog.text_field = path_text_field
                file_dialog.open()
            }
        }
    }
}
