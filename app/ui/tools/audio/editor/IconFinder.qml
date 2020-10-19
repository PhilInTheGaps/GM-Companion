import QtQuick 2.9
import QtQuick.Controls 2.2
import FontAwesome 2.0
import CustomComponents 1.0

import "../../../common"

CustomToolBarButton {
    id: icon_button
    property var text_field

    width: height
    anchors.margins: 0
    centering: true

    iconText: FontAwesome.ellipsisH
    iconFont: FontAwesome.familySolid
    toolTipText: qsTr("Select icon from files")
    pointSize: 12

    onClicked: {
        audio_editor_file_dialog.title = qsTr("Set Icon")

        audio_editor_file_dialog.foldersOnly = false
        audio_editor_file_dialog.textField = text_field
        audio_editor_file_dialog.folder = settings_manager.getPath("resources")
        audio_editor_file_dialog.replacePath = settings_manager.getPath(
                    "resources")
        audio_editor_file_dialog.open()
    }
}
