import QtQuick 2.9
import QtQuick.Controls 2.2
import "../../colors.js" as Colors

Rectangle {
    id: root
    property var addon: undefined

    color: Colors.base
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
                text: addon ? addon.name : "ERROR: COULD NOT LOAD ADDON"
                font.pointSize: 12
                font.bold: true
            }

            Label {
                text: addon
                      && addon.shortName.length > 0 ? "(" + addon.shortName + ")" : ""
                font.pointSize: 12
            }

            Rectangle {
                width: 1
                height: parent.height
                color: Colors.menuSeparator
            }

            Label {
                text: addon ? addon.author : ""
                font.pointSize: 12
            }
        }

        Label {
            anchors.left: parent.left
            anchors.right: parent.right

            text: addon ? addon.description : ""

            clip: true
            wrapMode: "WordWrap"
        }

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            height: 1
            visible: addon && addon.path.length > 0

            color: Colors.border
        }

        Label {
            anchors.left: parent.left
            anchors.right: parent.right

            text: addon ? addon.path : ""
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

            visible: addon && addon.isUpdateAvailable

            background: Rectangle {
                implicitWidth: 100
                implicitHeight: 40
                opacity: enabled ? 1 : 0.3
                color: Colors.button
            }

            onClicked: {
                addon_manager.updateAsync(addon)
            }
        }

        Button {
            text: addon && addon.enabled ? qsTr("Disable") : qsTr("Enable")
            anchors.horizontalCenter: parent.horizontalCenter
            visible: addon && addon.isInstalled
            enabled: addon && !addon.isInstalling

            onClicked: {
                addon_manager.setAddonEnabled(addon, !addon.enabled)
            }
        }

        Button {
            text: qsTr("Install")
            anchors.horizontalCenter: parent.horizontalCenter

            visible: addon && !addon.isInstalled

            onClicked: {
                addon_manager.installAsync(addon)
            }
        }

        Button {
            text: qsTr("Uninstall")
            anchors.horizontalCenter: parent.horizontalCenter

            enabled: addon && addon.isInstalled && !addon.isLocal
                     && !addon.isInstalling
            visible: addon && addon.isInstalled

            onClicked: {
                addon_manager.uninstall(addon)
            }
        }

        Label {
            text: addon ? "[ " + addon.version + " ]" : ""
            anchors.horizontalCenter: parent.horizontalCenter
            visible: addon && addon.version.length > 0
        }
    }

    Rectangle {
        id: overlay
        anchors.fill: parent
        color: "black"

        visible: addon && addon.isInstalling
        opacity: 0.5

        BusyIndicator {
            anchors.centerIn: parent
        }
    }
}
