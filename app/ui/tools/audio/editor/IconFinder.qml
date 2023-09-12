import QtQuick
import QtQuick.Controls
import IconFonts
import CustomComponents
import common
import "../../../common"

CustomToolBarButton {
    id: root

    required property TextField textField
    required property CustomFileDialog fileDialog

    width: height
    anchors.margins: 0
    centering: true

    iconText: FontAwesome.ellipsis
    iconFont: FontAwesome.fontSolid
    toolTipText: qsTr("Select icon from files")
    pointSize: 12

    onClicked: {
        root.fileDialog.title = qsTr("Set Icon")
        root.fileDialog.foldersOnly = false
        root.fileDialog.textField = textField
        root.fileDialog.folder = SettingsManager.getPath("resources")
        root.fileDialog.replacePath = SettingsManager.getPath("resources")
        root.fileDialog.open()
    }
}
