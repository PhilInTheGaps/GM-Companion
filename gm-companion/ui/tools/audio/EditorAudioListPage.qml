import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4 as Controls1_4

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
        width: parent.width
        height: parent.height
        spacing: 5

        Text {
            id: list_name_text
            text: qsTr("Audio List")
            color: color_scheme.textColor
        }

        Text {
            id: selected_file_text
            text: qsTr("Selected File:")
            color: color_scheme.textColor
        }

        Row {
            id: selected_file_properties_row
            width: parent.width
            spacing: 5

            Button {
                width: height

                Image {
                    source: "/icons/menu/arrow_up_dark.png"
                    width: parent.height * 0.8
                    height: parent.height * 0.8
                    sourceSize.width: width
                    sourceSize.height: height

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                onClicked: {
                    var row = table_view.currentRow

                    if (row - 1 > -1) {
                        moveFile(row, -1)

                        var rowData = table_model.get(row).file

                        table_model.remove(table_view.currentRow)
                        table_model.insert(row - 1, {
                                               file: rowData
                                           })
                        table_view.currentRow = row - 1
                    }
                }
            }

            Button {
                width: height

                Image {
                    source: "/icons/menu/arrow_down_dark.png"
                    width: parent.height * 0.8
                    height: parent.height * 0.8
                    sourceSize.width: width
                    sourceSize.height: height

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                onClicked: {
                    var row = table_view.currentRow

                    if (table_view.rowCount > row + 1) {
                        moveFile(row, 1)

                        var rowData = table_model.get(row).file

                        table_model.remove(table_view.currentRow)
                        table_model.insert(row + 1, {
                                               file: rowData
                                           })
                        table_view.currentRow = row + 1
                    }
                }
            }

            Button {
                text: qsTr("Remove")

                onClicked: {
                    var row = table_view.currentRow
                    removeFile(row)

                    table_model.remove(table_view.currentRow)

                    if (row > 0) {
                        table_view.selectedRow = row - 1
                    } else {
                        table_view.currentRow = row
                    }
                }
            }

            Button {
                text: qsTr("Save List")

                onClicked: saveList(type)
            }
        }

        Row {
            width: parent.width
            height: parent.height - parent.spacing * 3 - list_name_text.height
                    - selected_file_properties_row.height - selected_file_text.height
            spacing: 5

            Column {
                width: parent.width - parent.spacing - properties_column.width
                height: parent.height
                spacing: 5

                Controls1_4.TableView {
                    id: table_view
                    width: parent.width
                    height: parent.height

                    property int selectedRow: -1

                    model: ListModel {
                        id: table_model
                    }

                    Controls1_4.TableViewColumn {
                        id: table_column
                        title: qsTr("Files")
                        role: "file"
                        movable: false
                        width: table_view.width - 2
                    }

                    onRowCountChanged: {
                        currentRow = selectedRow
                        table_view.positionViewAtRow(currentRow,
                                                     ListView.Bottom)
                    }
                }
            }

            Column {
                id: properties_column
                width: parent.width / 4
                height: parent.height
                spacing: 5

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
