import QtQuick 2.9
import QtQuick.Controls 2.2

import gm.companion.addonelementmanager 1.0

Dialog {
    id: root

    signal addAddonElements(var names, var uris, var checked)

    title: qsTr("Add Element from Addon")

    AddonElementManager {
        id: manager

        onElementsChanged: element_repeater.model = getElements()
    }

    Component.onCompleted: {
        manager.setFolder(manager.getSpotifyFolders()[manager.getSpotifyFolders(
                                                          ).length - 1])
    }

    contentItem: Item {
        TabBar {
            id: tab_bar
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top

            Repeater {
                model: manager.getSpotifyFolders()

                TabButton {
                    text: modelData

                    onClicked: manager.setFolder(modelData)
                }
            }
        }

        ScrollView {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: tab_bar.bottom
            anchors.bottom: parent.bottom
            clip: true

            Column {
                id: column

                anchors.left: parent.left
                anchors.right: parent.right
                spacing: 10
                padding: 10

                Repeater {
                    id: element_repeater

                    Row {
                        spacing: 1
                        CheckBox {
                            checked: false
                            onCheckedChanged: manager.setAddElement(index,
                                                                    checked)
                        }

                        Text {
                            text: modelData

                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }
        }
    }

    standardButtons: Dialog.Ok | Dialog.Cancel

    onAccepted: {
        addAddonElements(manager.getElements(), manager.getURIs(),
                         manager.getChecked())

        element_repeater.model = []
        manager.resetChecked()
    }
}
