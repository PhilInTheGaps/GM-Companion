import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0

Item {
    id: root

    readonly property var paths: [{
            "name": qsTr("Audio Projects"),
            "setting": "audio"
        }, {
            "name": qsTr("Music"),
            "setting": "music"
        }, {
            "name": qsTr("Sounds"),
            "setting": "sounds"
        }, {
            "name": qsTr("Radios"),
            "setting": "radio"
        }, {
            "name": qsTr("Maps"),
            "setting": "maps"
        }, {
            "name": qsTr("Characters"),
            "setting": "characters"
        }, {
            "name": qsTr("Notes"),
            "setting": "notes"
        }, {
            "name": qsTr("Shops"),
            "setting": "shops"
        }, {
            "name": qsTr("Resources"),
            "setting": "resources"
        }]

    signal fileModeChanged

    onFileModeChanged: file_manager.updateFileAccess()

    Column {
        id: type_list
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 5

        Repeater {
            model: [{
                    "name": "Local",
                    "setting": "local",
                    "icon": FontAwesome.hdd,
                    "iconFont": FontAwesome.familySolid
                }, {
                    "name": "GoogleDrive",
                    "setting": "GoogleDrive",
                    "icon": FontAwesome.googleDrive,
                    "iconFont": FontAwesome.familyBrands
                }, {
                    "name": "NextCloud",
                    "setting": "NextCloud",
                    "icon": FontAwesome.cloud,
                    "iconFont": FontAwesome.familySolid
                }]

            CustomRadioButton {
                text: modelData.name
                iconText: modelData.icon
                iconFont: modelData.iconFont
                checked: settings_manager.getSetting(
                             "cloudMode", "local") === modelData.setting

                onClicked: {
                    if (checked) {
                        settings_manager.setSetting("cloudMode",
                                                    modelData.setting)
                        fileModeChanged()
                    }
                }
            }
        }
    }

    Flickable {
        id: flickable
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: type_list.right
        anchors.right: parent.right
        anchors.margins: 10

        clip: true
        contentWidth: -1
        contentHeight: main_column.implicitHeight

        ScrollBar.vertical: ScrollBar {
            id: scroll_bar
            visible: flickable.contentHeight > flickable.height
        }

        Column {
            id: main_column
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: scroll_bar.visible ? scroll_bar.width + 5 : 0

            spacing: 5

            Repeater {
                model: root.paths

                Column {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    spacing: 5

                    Label {
                        text: modelData.name
                    }

                    Item {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: path_text_field.height

                        TextField {
                            id: path_text_field
                            anchors.left: parent.left
                            anchors.right: dots_button.left
                            anchors.rightMargin: 5

                            function loadPath() {
                                text = settings_manager.getPath(
                                            modelData.setting)
                            }

                            function savePath() {
                                settings_manager.setPath(modelData.setting,
                                                         text)
                            }

                            Component.onCompleted: loadPath()
                            onTextEdited: savePath()

                            Connections {
                                target: root

                                function onFileModeChanged() {
                                    path_text_field.loadPath()
                                }
                            }
                        }

                        CustomButton {
                            id: dots_button
                            buttonText: "..."
                            usesFixedWidth: false
                            anchors.right: parent.right
                            width: height
                            centering: true

                            onClicked: {
                                file_dialog.title = modelData.name
                                file_dialog.foldersOnly = true
                                file_dialog.folder = path_text_field.text
                                file_dialog.textField = path_text_field
                                file_dialog.open()
                            }
                        }
                    }
                }
            }
        }
    }
}
