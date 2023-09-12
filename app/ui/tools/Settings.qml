pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import common
import "../common"

Item {
    id: root

    CustomFileDialog {
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
                        "source": "settings/PathsPage.qml",
                        "properties": {
                            "fileDialog": file_dialog
                        }
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
                    required property var modelData

                    buttonText: modelData.name
                    iconText: modelData.icon

                    anchors.left: parent ? parent.left : undefined
                    anchors.right: parent ? parent.right : undefined

                    onClicked: loader.setSource(modelData.source,
                                                modelData.properties)
                }
            }
        }

        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.margins: 10
            text: "Version: " + UpdateManager.getCurrentVersion()
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
