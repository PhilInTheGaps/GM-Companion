import QtQuick 2.9
import QtQuick.Controls 2.2

Item {
    id: root

    Label {
        id: addon_text
        width: parent.width
        text: qsTr("The following addons are available:")
        color: color_scheme.textColor
        font.pointSize: 12
        clip: true
        wrapMode: Text.WordWrap
    }

    ScrollView {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: addon_text.bottom
        anchors.topMargin: 10
        clip: true

        Column {
            id: addon_column
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 10
            topPadding: 10

            Repeater {
                model: addon_manager.addonNames

                AddonItem {
                    addon: modelData
                    description: addon_manager.getAddonDescriptions()[index]
                    folder: addon_manager.getAddonPathNames()[index]
                    addon_enabled: addon_manager.getAddonEnabledList()[index]
                }
            }
        }
    }
}
