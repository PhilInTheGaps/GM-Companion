import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4

import gm.companion.charactersaveload 1.0
import gm.companion.charactersaveload 1.0

TableView {
    id: table_view
    horizontalScrollBarPolicy: -1
    selectionMode: SelectionMode.SingleSelection

    flickableItem.interactive: true
    clip: true

    property real col1_width_divisor: 4
    property int col2_width: viewport.width - col1.width

    property int row_count: 10
    property int row_height: 30

    property string character_name: ""
    property string table_name: ""

    property string col1_name: "Column 1"
    property string col2_name: "Column 2"
    property bool columns_visible: true
    property var col1_list: [""]
    property var col2_list: [""]
    property int column_count: 2

    Component.onCompleted: fillList()

    headerVisible: columns_visible

    function save() {
        console.log("Saving Table " + table_name + " ...")

        var list = new Array(0) // List of Column Lists

        for (var i = 0; i < column_count; i++) {
            var row_values = new Array(0) // Column content list

            for (var j = 0; j < row_count; j++) {
                if (i === 0)
                    row_values.push(table_model.get(j).col1)
                else if (i === 1) {
                    row_values.push(table_model.get(j).col2)
                }
            }

            list.push(row_values)
        }

        save_load.saveTable(character_name, table_name, list)
    }

    CharacterSaveLoad {
        id: save_load
    }

    TableViewColumn {
        id: col1
        title: col1_name
        role: "col1"
        movable: false
        width: table_view.viewport.width / col1_width_divisor
    }

    TableViewColumn {
        id: col2
        title: col2_name
        role: "col2"
        movable: false
        width: col2_width
    }

    rowDelegate: Rectangle {
        height: row_height
    }

    model: ListModel {
        id: table_model
    }

    // Write content to table
    function fillList() {
        table_model.clear()

        if (col1_list.length > row_count)
            row_count = col1_list.length
        if (col2_list.length > row_count)
            row_count = col2_list.length

        console.log(col2_list.length)

        for (var i = 0; i < row_count; i++) {
            var col1_text = ""
            var col2_text = ""

            if (i < col1_list.length)
                col1_text = col1_list[i]

            if (i < col2_list.length)
                col2_text = col2_list[i]

            table_model.append({
                                   col1: col1_text,
                                   col2: col2_text
                               })
        }
    }

    itemDelegate: Rectangle {
        border.color: "grey"
        border.width: 1
        height: table_view.row_height

        Text {
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
            }

            padding: 5

            color: "black"
            text: styleData.value
        }

        MouseArea {
            id: cell_mouse_area
            anchors.fill: parent
            onClicked: {
                loader.visible = true
                loader.item.forceActiveFocus()
            }
        }

        Loader {
            id: loader
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
            }
            height: parent.height
            width: parent.width
            visible: false
            sourceComponent: visible ? input : undefined

            Component {
                id: input
                TextField {
                    anchors {
                        fill: parent
                    }
                    text: styleData.column === 0 ? table_model.get(
                                                       styleData.row).col1 : table_model.get(
                                                       styleData.row).col2

                    onAccepted: {
                        writeText()
                        loader.visible = false
                    }

                    onActiveFocusChanged: {
                        if (!activeFocus) {
                            writeText()
                            loader.visible = false
                        }
                    }

                    function writeText() {
                        if (styleData.column === 0)
                            table_model.get(styleData.row).col1 = text
                        else if (styleData.column === 1)
                            table_model.get(styleData.row).col2 = text
                    }
                }
            }
        }
    }
}
