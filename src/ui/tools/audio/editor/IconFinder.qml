import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2

import FontAwesome 2.0

Button {
    id: icon_button
    property var text_field
    readonly property string resourcesPath: settings_tool.getPath("resources")

    anchors.top: parent.top
    anchors.bottom: parent.bottom
    width: height

    background: Rectangle {
        color: color_scheme.menuColor
    }

    Text {
        text: FontAwesome.ellipsisH
        font.family: FontAwesome.familySolid
        font.pixelSize: height
        anchors.fill: parent
        anchors.margins: 10
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        color: color_scheme.toolbarTextColor
    }

    onClicked: {
        file_dialog.folder = (platform.isWindows ? "file:///" : "file://") + resourcesPath
        file_dialog.open()
    }

    FileDialog {
        id: file_dialog
        title: qsTr("Set Icon")

        selectFolder: false

        onAccepted: {
            text_field.text = fileUrl.toString().replace((platform.isWindows ? "file:///" : "file://") + resourcesPath, "")

        }
    }
}
