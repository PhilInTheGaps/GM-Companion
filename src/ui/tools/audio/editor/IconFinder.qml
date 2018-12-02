import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
import gm.companion.platforms 1.0

Button {
    id: icon_button
    property var text_field
    readonly property string resourcesPath: settings_tool.getPath("resources")

    anchors.top: parent.top
    anchors.bottom: parent.bottom
    width: height

    text: "..."

    onClicked: {
        file_dialog.folder = "file://" + resourcesPath
        file_dialog.open()
    }

    PlatformDetails {
        id: platform_details
    }

    FileDialog {
        id: file_dialog
        title: qsTr("Set Icon")

        selectFolder: false

        onAccepted: {

            if (platform_details.isWindows)
                text_field.text = fileUrl.toString().replace(
                            "file:///" + resourcesPath, "")
            else
                text_field.text = fileUrl.toString().replace(
                            "file://" + resourcesPath, "")
        }
    }
}
