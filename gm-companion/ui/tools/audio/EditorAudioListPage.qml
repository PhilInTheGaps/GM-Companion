import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4

import "../../fontawesome"
import gm.companion.colorscheme 1.0

Page {
    id: audio_list_page

    signal removeFile(int fileIndex)
    signal saveList(int type)
    signal moveFile(int index, int positions)
    signal changeMode(int mode)
    signal changeIcon(string path)

    property string resourcesPath
    property int type

    function setName(name) {
        list_name_text.text = name
    }

    function populateTable(files) {
        table_model.clear()

        for (var i = 0; i < files.length; i++) {
            table_model.append({
                                   file: files[i]
                               })
        }
    }

    function setPlaybackMode(mode) {
        switch (mode) {
        case 1:
            mode_radio_1.checked = true
            break
        case 2:
            mode_radio_2.checked = true
            break
        case 3:
            mode_radio_3.checked = true
            break
        }
    }

    function setIconPath(path) {
        icon_path_textfield.text = path
        icon_image.source = "file://" + resourcesPath + path
    }

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    Column {
        anchors.fill: parent
        spacing: 5

        Text {
            id: list_name_text
            text: qsTr("Audio List")
            color: color_scheme.textColor
        }

        Row {
            width: parent.width
            height: parent.height - parent.spacing - list_name_text.height

            Column {
                width: parent.width - parent.spacing - properties_column.width
                height: parent.height
                spacing: 5

                ListView {
                    id: table_view
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: parent.height

                    property int selectedRow: -1

                    clip: true
                    spacing: 0

                    model: ListModel {
                        id: table_model
                    }

                    ScrollBar.vertical: ScrollBar {
                    }

                    delegate: Rectangle {
                        height: delegate_row.height
                        anchors.left: parent.left
                        anchors.right: parent.right
                        color: "transparent"

                        MouseArea {
                            id: mouse_area
                            anchors.fill: parent
                            hoverEnabled: true

                            z: 2
                            onClicked: mouse.accepted = false
                            onPressed: mouse.accepted = false
                            onReleased: mouse.accepted = false
                            onDoubleClicked: mouse.accepted = false
                            onPositionChanged: mouse.accepted = false
                            onPressAndHold: mouse.accepted = false
                        }

                        Row {
                            id: delegate_row
                            padding: 5
                            leftPadding: 10
                            rightPadding: 10
                            spacing: 10
                            anchors.left: parent.left
                            anchors.right: parent.right
                            height: 35

                            Text {
                                text: file
                                color: color_scheme.textColor
                                width: parent.width - parent.leftPadding
                                       - parent.rightPadding - x_button.width
                                       - up_down_column.width - parent.spacing * 2
                                clip: true
                                elide: Text.ElideRight
                                anchors.verticalCenter: parent.verticalCenter
                                font.pointSize: 10
                                font.bold: true
                            }

                            Column {
                                id: up_down_column
                                height: parent.height - parent.topPadding - parent.bottomPadding
                                width: height

                                visible: mouse_area.containsMouse

                                Button {
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    height: parent.height / 2
                                    hoverEnabled: true

                                    background: Rectangle {
                                        color: "transparent"
                                    }

                                    Icon {
                                        icon: icons.fa_angle_up
                                        pointSize: 20
                                        anchors.centerIn: parent
                                        color: parent.pressed ? "grey" : (parent.hovered ? "lightgrey" : color_scheme.primaryButtonColor)
                                    }

                                    onClicked: {
                                        moveFile(index, -1)
                                        table_model.move(index, index - 1, 1)
                                    }
                                }

                                Button {
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    height: parent.height / 2
                                    hoverEnabled: true

                                    background: Rectangle {
                                        color: "transparent"
                                    }

                                    Icon {
                                        icon: icons.fa_angle_down
                                        pointSize: 20
                                        anchors.centerIn: parent
                                        color: parent.pressed ? "grey" : (parent.hovered ? "lightgrey" : color_scheme.primaryButtonColor)
                                    }

                                    onClicked: {
                                        moveFile(index, 1)
                                        table_model.move(index, index + 1, 1)
                                    }
                                }
                            }

                            Button {
                                id: x_button
                                height: parent.height - parent.topPadding - parent.bottomPadding
                                width: height
                                hoverEnabled: true

                                visible: mouse_area.containsMouse

                                background: Rectangle {
                                    color: "transparent"
                                }

                                Icon {
                                    icon: icons.fa_times
                                    pointSize: 20
                                    anchors.centerIn: parent
                                    color: parent.pressed ? "grey" : (parent.hovered ? "lightgrey" : color_scheme.primaryButtonColor)
                                }

                                onClicked: {
                                    removeFile(index)
                                    table_model.remove(index)
                                }
                            }
                        }
                    }
                }
            }

            Column {
                id: properties_column
                width: parent.width / 4
                height: parent.height
                spacing: 5

                Button {
                    text: qsTr("Save List")
                    onClicked: saveList(type)
                    width: parent.width
                }

                Text {
                    text: qsTr("Playback Mode")
                    color: color_scheme.textColor
                }

                RadioButton {
                    id: mode_radio_0
                    checked: true
                    text: qsTr("Shuffle List")
                    width: parent.width

                    background: Rectangle {
                        color: "#e0e0e0"
                    }

                    ToolTip.text: qsTr("Playlist is shuffled before playing. Loops.")
                    ToolTip.visible: hovered
                    hoverEnabled: true

                    onCheckedChanged: {
                        if (checked)
                            changeMode(0)
                    }
                }

                RadioButton {
                    id: mode_radio_1
                    checked: false
                    text: qsTr("Random Mode")
                    width: parent.width

                    background: Rectangle {
                        color: "#e0e0e0"
                    }

                    ToolTip.text: qsTr("Files are played randomly, does not stop.")
                    ToolTip.visible: hovered
                    hoverEnabled: true

                    onCheckedChanged: {
                        if (checked)
                            changeMode(1)
                    }
                }

                RadioButton {
                    id: mode_radio_2
                    checked: false
                    text: qsTr("Loop List")
                    width: parent.width

                    background: Rectangle {
                        color: "#e0e0e0"
                    }

                    ToolTip.text: qsTr("Playlist loops in sequential order.")
                    ToolTip.visible: hovered
                    hoverEnabled: true

                    onCheckedChanged: {
                        if (checked)
                            changeMode(2)
                    }
                }

                RadioButton {
                    id: mode_radio_3
                    checked: false
                    text: qsTr("Sequential Order")
                    width: parent.width

                    background: Rectangle {
                        color: "#e0e0e0"
                    }

                    ToolTip.text: qsTr("Playlist is played in set order. Does not loop.")
                    ToolTip.visible: hovered
                    hoverEnabled: true

                    onCheckedChanged: {
                        if (checked)
                            changeMode(3)
                    }
                }

                Button {
                    text: qsTr("Remove Missing Files")
                    width: parent.width

                    onClicked: editor_tool.removeMissingFiles()
                }

                Text {
                    text: qsTr("List Icon")
                    width: parent.width
                    color: color_scheme.textColor
                }

                Row {
                    width: parent.width
                    spacing: 5

                    TextField {
                        id: icon_path_textfield
                        placeholderText: qsTr("Icon Path")
                        width: parent.width
                        selectByMouse: true

                        onTextChanged: {
                            changeIcon(text)
                            icon_image.source = "file://" + resourcesPath + text
                        }
                    }
                }

                Image {
                    id: icon_image

                    width: parent.width
                    height: width
                }
            }
        }
    }
}
