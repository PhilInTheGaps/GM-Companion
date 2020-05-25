import QtQuick 2.6
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0

Rectangle {
    id: root
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.rightMargin: 5
    height: column.height

    border.width: 2
    border.color: "#7289DA"
    color: palette.base

    Column {
        id: column
        anchors.left: parent.left
        anchors.right: parent.right
        padding: 10
        spacing: 5

        Row {
            height: channel_label.height
            spacing: 5

            Label {
                text: FontAwesome.discord
                font.family: FontAwesome.familyBrands
                color: "#7289DA"
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
            }

            Label {
                text: qsTr("Discord")
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        Label {
            id: channel_label
            text: discord_service.channel !== "" ? discord_service.channel : qsTr(
                                                       "no channel")
        }
    }

    CustomButton {
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.verticalCenter: parent.verticalCenter

        iconText: FontAwesome.timesCircle
        iconColor: "red"
        usesFixedWidth: false
        transparentBackground: true

        onClicked: {
            settings_manager.setSetting("enabled", false, "Discord")
            discord_service.enabled = false
        }
    }

    visible: discord_service.enabled
}
