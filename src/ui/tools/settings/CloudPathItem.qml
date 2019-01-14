import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2

Column {
    id: column
    property string path_type
    property string setting
    spacing: 5

    anchors.left: parent.left
    anchors.right: parent.right
    anchors.margins: parent.padding

    Text {
        text: path_type
        color: color_scheme.textColor
    }

    TextField {
        id: path_text_field
        anchors.left: parent.left
        anchors.right: parent.right

        text: settings_tool.getGooglePath(setting)
        selectByMouse: true

        onTextChanged: {
            settings_tool.setGooglePath(setting, text)
        }
    }
}
