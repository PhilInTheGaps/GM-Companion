import QtQuick 2.9
import QtQuick.Controls 2.2

Dialog {
    id: root

    signal addAddonElements(var names, var uris, var checked)

    title: qsTr("Add Element from Addon")

    Connections {
        target: audio_addon_element_manager
    }

    contentItem: Item {
        TabBar {
            id: tab_bar
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top

            Repeater {
                model: audio_addon_element_manager.spotifyFolders

                TabButton {
                    text: modelData

                    onClicked: audio_addon_element_manager.setFolder(modelData)
                }
            }
        }

        ScrollView {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: tab_bar.bottom
            anchors.bottom: parent.bottom
            clip: true
            contentHeight: column.implicitHeight

            Column {
                id: column

                anchors.left: parent.left
                anchors.right: parent.right
                spacing: 10
                padding: 10

                Repeater {
                    id: element_repeater
                    model: audio_addon_element_manager.elementNames

                    Row {
                        spacing: 1

                        CheckBox {
                            checked: false
                            onCheckedChanged: audio_addon_element_manager.setAddElement(
                                                  index, checked)
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
        audio_addon_element_manager.addElements(
                    new_thing_dialog.subscenarioChecked,
                    new_thing_dialog.subscenarioIndex - 1)
        audio_addon_element_manager.resetChecked()
    }
}
