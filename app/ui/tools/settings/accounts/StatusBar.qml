import QtQuick
import QtQuick.Controls
import IconFonts
import services as Services
import common as Common
import "../../.."

Rectangle {
    id: root
    height: Sizes.toolbarHeight

    property Services.Status status: undefined

    visible: status.message !== ""

    Row {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        spacing: 10
        padding: 10

        Label {
            id: icon_label
            font.family: FontAwesome.fontSolid.family
            font.styleName: FontAwesome.fontSolid.styleName
            font.pointSize: 12
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter

            text: {
                switch (root.status.type) {
                case 1:
                    FontAwesome.circleCheck;
                    break;
                case 2:
                    FontAwesome.triangleExclamation;
                    break;
                case 3:
                    FontAwesome.circleExclamation;
                    break;
                default:
                    FontAwesome.circleInfo;
                }
            }

            color: {
                switch (root.status.type) {
                case 1:
                    Common.SettingsManager.colors.success;
                    break;
                case 2:
                    Common.SettingsManager.colors.warning;
                    break;
                case 3:
                    Common.SettingsManager.colors.error;
                    break;
                default:
                    Common.SettingsManager.colors.info;
                }
            }
        }

        Label {
            id: text_label
            text: root.status.message
            font.pointSize: 12
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    border.color: {
        switch (root.status.type) {
        case 2:
            Common.SettingsManager.colors.warning;
            break;
        case 3:
            Common.SettingsManager.colors.error;
            break;
        default:
            palette.highlight;
        }
    }

    border.width: 1
    color: "transparent"
}
