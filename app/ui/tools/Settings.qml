import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import IconFonts
import "../common"
import "./settings"

Item {
    id: settings_page

    FileDialog {
        id: file_dialog
    }

    Rectangle {
        id: page_list
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: 175

        color: palette.dark

        Column {
            anchors.fill: parent
            anchors.margins: 5

            Repeater {
                id: page_repeater
                model: [{
                        "name": qsTr("General"),
                        "icon": FontAwesome.screwdriverWrench,
                        "source": "settings/GeneralPage.qml"
                    }, {
                        "name": qsTr("Accounts"),
                        "icon": FontAwesome.users,
                        "source": "settings/AccountsPage.qml"
                    }, {
                        "name": qsTr("Paths"),
                        "icon": FontAwesome.folderOpen,
                        "source": "settings/PathsPage.qml"
                    }, {
                        "name": qsTr("Addons"),
                        "icon": FontAwesome.puzzlePiece,
                        "source": "settings/AddonsPage.qml"
                    }, {
                        "name": qsTr("Info"),
                        "icon": FontAwesome.circleInfo,
                        "source": "settings/InfoPage.qml"
                    }]
                CustomButton {
                    buttonText: modelData.name
                    iconText: modelData.icon

                    anchors.left: parent.left
                    anchors.right: parent.right

                    onClicked: loader.setSource(modelData.source)
                }
            }
        }

        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.margins: 10
            text: "Version: " + update_manager.getCurrentVersion()
        }
    }

    Loader {
        id: loader

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: page_list.right
        anchors.right: parent.right

        asynchronous: true
        source: page_repeater.model[0].source
    }
}
