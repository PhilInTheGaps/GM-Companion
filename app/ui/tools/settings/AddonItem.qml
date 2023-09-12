import QtQuick
import QtQuick.Controls
import src
import "../.."

Rectangle {
    id: root
    property Addon addon: undefined

    color: palette.base
    border.color: addon && addon.enabled ? Colors.borderFocus : Colors.border
    border.width: 1

    height: Math.max(left_column.height, right_column.height) + 20

    Column {
        id: left_column
        anchors.left: parent.left
        anchors.right: right_column.left
        anchors.top: parent.top
        anchors.margins: 10
        spacing: 10

        Row {
            anchors.left: parent.left
            anchors.right: parent.right

            spacing: 10

            Label {
                text: root.addon ? root.addon.name : "ERROR: COULD NOT LOAD ADDON"
                font.pointSize: 12
                font.bold: true
            }

            Label {
                text: root.addon
                      && root.addon.shortName.length > 0 ? "(" + root.addon.shortName + ")" : ""
                font.pointSize: 12
            }

            Rectangle {
                width: 1
                height: parent.height
                color: Colors.menuSeparator
            }

            Label {
                text: root.addon ? root.addon.author : ""
                font.pointSize: 12
            }
        }

        Label {
            anchors.left: parent.left
            anchors.right: parent.right

            text: root.addon ? root.addon.description : ""

            clip: true
            wrapMode: Label.WordWrap
        }

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            height: 1
            visible: root.addon && root.addon.path.length > 0

            color: Colors.border
        }

        Label {
            anchors.left: parent.left
            anchors.right: parent.right

            text: root.addon ? root.addon.path : ""
        }
    }

    Column {
        id: right_column
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 10
        spacing: 10

        Button {
            text: qsTr("Update")
            anchors.horizontalCenter: parent.horizontalCenter

            visible: root.addon && root.addon.isUpdateAvailable

            background: Rectangle {
                implicitWidth: 100
                implicitHeight: 40
                opacity: enabled ? 1 : 0.3
                color: palette.button
            }

            onClicked: {
                AddonManager.updateAsync(root.addon)
            }
        }

        Button {
            text: root.addon && root.addon.enabled ? qsTr("Disable") : qsTr("Enable")
            anchors.horizontalCenter: parent.horizontalCenter
            visible: root.addon && root.addon.isInstalled
            enabled: root.addon && !root.addon.isInstalling

            onClicked: {
                AddonManager.setAddonEnabled(root.addon, !root.addon.enabled)
            }
        }

        Button {
            text: qsTr("Install")
            anchors.horizontalCenter: parent.horizontalCenter

            visible: root.addon && !root.addon.isInstalled

            onClicked: {
                AddonManager.installAsync(root.addon)
            }
        }

        Button {
            text: qsTr("Uninstall")
            anchors.horizontalCenter: parent.horizontalCenter

            enabled: root.addon && root.addon.isInstalled && !root.addon.isLocal
                     && !root.addon.isInstalling
            visible: root.addon && root.addon.isInstalled

            onClicked: {
                AddonManager.uninstall(root.addon)
            }
        }

        Label {
            text: root.addon ? "[ " + root.addon.version + " ]" : ""
            anchors.horizontalCenter: parent.horizontalCenter
            visible: root.addon && root.addon.version.length > 0
        }
    }

    Rectangle {
        id: overlay
        anchors.fill: parent
        color: "black"

        visible: root.addon && root.addon.isInstalling
        opacity: 0.5

        BusyIndicator {
            anchors.centerIn: parent
        }
    }
}
