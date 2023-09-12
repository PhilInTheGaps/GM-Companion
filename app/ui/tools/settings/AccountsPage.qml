pragma ComponentBehavior: Bound
import QtQuick
import CustomComponents
import IconFonts

Item {
    id: root

    readonly property var accounts: [{
            "name": "Spotify",
            "icon": FontAwesome.spotify,
            "iconFont": FontAwesome.fontBrands,
            "source": "./accounts/SpotifyAccount.qml"
        }, {
            "name": "GoogleDrive",
            "icon": FontAwesome.googleDrive,
            "iconFont": FontAwesome.fontBrands,
            "source": "./accounts/GoogleDriveAccount.qml"
        }, {
            "name": "NextCloud",
            "icon": FontAwesome.cloud,
            "iconFont": FontAwesome.fontSolid,
            "source": "./accounts/NextCloudAccount.qml"
        }]

    Column {
        id: accounts_list
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 5
        width: 175

        Repeater {
            model: root.accounts

            CustomButton {
                // TODO: type
                required property var modelData

                buttonText: modelData.name
                iconText: modelData.icon
                iconFont: modelData.iconFont

                anchors.left: parent.left
                anchors.right: parent.right
                backgroundColor: "transparent"

                onClicked: loader.source = modelData.source
            }
        }
    }

    Loader {
        id: loader
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: accounts_list.right
        anchors.right: parent.right
        anchors.margins: 10
        anchors.topMargin: 5
        asynchronous: true

        source: root.accounts[0].source
    }
}
