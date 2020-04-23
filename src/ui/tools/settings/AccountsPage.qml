import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "./accounts"

Item {
    id: root

    readonly property var accounts: [{
            "name": "Spotify",
            "icon": FontAwesome.spotify,
            "iconFont": FontAwesome.familyBrands,
            "source": "./accounts/SpotifyAccount.qml"
        }, {
            "name": "GoogleDrive",
            "icon": FontAwesome.googleDrive,
            "iconFont": FontAwesome.familyBrands,
            "source": "./accounts/GoogleDriveAccount.qml"
        }]


    /*
    {
        "name": "NextCloud",
        "icon": FontAwesome.cloud,
        "iconFont": FontAwesome.familySolid,
        "source": "./accounts/NextCloudAccount.qml"
    }
    */
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
        anchors.topMargin: 20
        asynchronous: true

        source: root.accounts[0].source
    }
}
