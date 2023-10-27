pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import filesystem
import common
import "../../common"

Item {
    id: root

    required property CustomFileDialog fileDialog

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

    onFileModeChanged: FileAccessSwitcher.updateFileAccess()

    Column {
        id: type_list
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 5

        Repeater {
//            {
//                "name": "GoogleDrive",
//                "setting": "GoogleDrive",
//                "icon": FontAwesome.googleDrive,
//                "iconFont": FontAwesome.fontBrands
//            },

            model: [{
                    "name": "Local",
                    "setting": "local",
                    "icon": FontAwesome.hardDrive,
                    "iconFont": FontAwesome.fontSolid
                },  {
                    "name": "NextCloud",
                    "setting": "NextCloud",
                    "icon": FontAwesome.cloud,
                    "iconFont": FontAwesome.fontSolid
                }]

            CustomRadioButton {
                required property var modelData

                text: modelData.name
                iconText: modelData.icon
                iconFont: modelData.iconFont
                checked: SettingsManager.cloudMode === modelData.setting

                onClicked: {
                    if (checked) {
                        SettingsManager.cloudMode = modelData.setting
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
                    id: path_box

                    required property var modelData

                    anchors.left: parent.left
                    anchors.right: parent.right
                    spacing: 5

                    Label {
                        text: path_box.modelData.name
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
                                text = SettingsManager.getPath(
                                            path_box.modelData.setting)
                            }

                            function savePath() {
                                SettingsManager.setPath(path_box.modelData.setting, text)
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
                                root.fileDialog.title = path_box.modelData.name
                                root.fileDialog.foldersOnly = true
                                root.fileDialog.folder = path_text_field.text
                                root.fileDialog.textField = path_text_field
                                root.fileDialog.open()
                            }
                        }
                    }
                }
            }
        }
    }
}
