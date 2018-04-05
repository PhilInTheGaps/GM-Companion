import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4

TableView {
    id: table_view
    horizontalScrollBarPolicy: -1
    selectionMode: SelectionMode.SingleSelection

    flickableItem.interactive: true
    clip: true

    property real col1_width_divisor: column_count
    property real col2_width_divisor: column_count
    property real col3_width_divisor: column_count
    property real col4_width_divisor: column_count
    property real col5_width_divisor: column_count
    property real col6_width_divisor: column_count
    property real col7_width_divisor: column_count
    property real col8_width_divisor: column_count
    property real col9_width_divisor: column_count
    property real col10_width_divisor: column_count

    property int column_count: 2
    property int row_count: 10
    property int row_height: 30

    property string character_name: ""
    property string table_name: ""

    property string col1_name: "Column 1"
    property string col2_name: "Column 2"
    property string col3_name: "Column 3"
    property string col4_name: "Column 4"
    property string col5_name: "Column 5"
    property string col6_name: "Column 6"
    property string col7_name: "Column 7"
    property string col8_name: "Column 8"
    property string col9_name: "Column 9"
    property string col10_name: "Column 10"

    property bool columns_visible: true
    property var col1_list: [""]
    property var col2_list: [""]
    property var col3_list: [""]
    property var col4_list: [""]
    property var col5_list: [""]
    property var col6_list: [""]
    property var col7_list: [""]
    property var col8_list: [""]
    property var col9_list: [""]
    property var col10_list: [""]

    Component.onCompleted: fillList()

    headerVisible: columns_visible

    function save() {
        var return_values = new Array(0)
        var list = new Array(0) // List of Column Lists

        for (var i = 0; i < column_count; i++) {
            var row_values = new Array(0) // Column content list

            for (var j = 0; j < row_count; j++) {

                switch (i) {
                case 0:
                    row_values.push(table_model.get(j).col1)
                    break
                case 1:
                    row_values.push(table_model.get(j).col2)
                    break
                case 2:
                    row_values.push(table_model.get(j).col3)
                    break
                case 3:
                    row_values.push(table_model.get(j).col4)
                    break
                case 4:
                    row_values.push(table_model.get(j).col5)
                    break
                case 5:
                    row_values.push(table_model.get(j).col6)
                    break
                case 6:
                    row_values.push(table_model.get(j).col7)
                    break
                case 7:
                    row_values.push(table_model.get(j).col8)
                    break
                case 8:
                    row_values.push(table_model.get(j).col9)
                    break
                case 9:
                    row_values.push(table_model.get(j).col10)
                    break
                }
            }

            list.push(row_values)
        }

        return_values.push(table_name)
        return_values.push(list)

        return return_values
    }

    TableViewColumn {
        id: col1
        title: col1_name
        role: "col1"
        movable: false
        width: table_view.viewport.width / col1_width_divisor
        visible: column_count > 0 ? true : false
    }

    TableViewColumn {
        id: col2
        title: col2_name
        role: "col2"
        movable: false
        width: column_count == 2 ? table_view.viewport.width
                                   - col1.width : table_view.viewport.width / col2_width_divisor
        visible: column_count > 1 ? true : false
    }

    TableViewColumn {
        id: col3
        title: col3_name
        role: "col3"
        movable: false
        width: column_count == 3 ? table_view.viewport.width - col1.width
                                   - col2.width : table_view.viewport.width / col3_width_divisor
        visible: column_count > 2 ? true : false
    }

    TableViewColumn {
        id: col4
        title: col4_name
        role: "col4"
        movable: false
        width: column_count == 4 ? table_view.viewport.width - col1.width - col2.width
                                   - col3.width : table_view.viewport.width / col4_width_divisor
        visible: column_count > 3 ? true : false
    }

    TableViewColumn {
        id: col5
        title: col5_name
        role: "col5"
        movable: false
        width: column_count == 5 ? table_view.viewport.width - col1.width - col2.width - col3.width
                                   - col4.width : table_view.viewport.width / col5_width_divisor
        visible: column_count > 4 ? true : false
    }

    TableViewColumn {
        id: col6
        title: col6_name
        role: "col6"
        movable: false
        width: column_count == 6 ? table_view.viewport.width - col1.width
                                   - col2.width - col3.width - col4.width
                                   - col5.width : table_view.viewport.width / col6_width_divisor
        visible: column_count > 5 ? true : false
    }

    TableViewColumn {
        id: col7
        title: col7_name
        role: "col7"
        movable: false
        width: column_count == 7 ? table_view.viewport.width - col1.width - col2.width
                                   - col3.width - col4.width - col5.width
                                   - col6.width : table_view.viewport.width / col7_width_divisor
        visible: column_count > 6 ? true : false
    }

    TableViewColumn {
        id: col8
        title: col8_name
        role: "col8"
        movable: false
        width: column_count == 8 ? table_view.viewport.width - col1.width - col2.width
                                   - col3.width - col4.width - col5.width - col6.width
                                   - col7.width : table_view.viewport.width / col8_width_divisor
        visible: column_count > 7 ? true : false
    }

    TableViewColumn {
        id: col9
        title: col9_name
        role: "col9"
        movable: false
        width: column_count
               == 9 ? table_view.viewport.width - col1.width - col2.width - col3.width
                      - col4.width - col5.width - col6.width - col7.width
                      - col8.width : table_view.viewport.width / col9_width_divisor
        visible: column_count > 8 ? true : false
    }

    TableViewColumn {
        id: col10
        title: col10_name
        role: "col10"
        movable: false
        width: column_count
               == 10 ? table_view.viewport.width - col1.width - col2.width - col3.width
                       - col4.width - col5.width - col6.width - col7.width - col8.width
                       - col9.width : table_view.viewport.width / col10_width_divisor
        visible: column_count > 9 ? true : false
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

        for (var i = 0; i < row_count; i++) {
            var col1_text = ""
            var col2_text = ""
            var col3_text = ""
            var col4_text = ""
            var col5_text = ""
            var col6_text = ""
            var col7_text = ""
            var col8_text = ""
            var col9_text = ""
            var col10_text = ""

            if (i < col1_list.length)
                col1_text = col1_list[i]

            if (i < col2_list.length)
                col2_text = col2_list[i]

            if (i < col3_list.length)
                col3_text = col3_list[i]

            if (i < col4_list.length)
                col4_text = col4_list[i]

            if (i < col5_list.length)
                col5_text = col5_list[i]

            if (i < col6_list.length)
                col6_text = col6_list[i]

            if (i < col7_list.length)
                col7_text = col7_list[i]

            if (i < col8_list.length)
                col8_text = col8_list[i]

            if (i < col9_list.length)
                col9_text = col9_list[i]

            if (i < col10_list.length)
                col10_text = col10_list[i]

            table_model.append({
                                   col1: col1_text,
                                   col2: col2_text,
                                   col3: col3_text,
                                   col4: col4_text,
                                   col5: col5_text,
                                   col6: col6_text,
                                   col7: col7_text,
                                   col8: col8_text,
                                   col9: col9_text,
                                   col10: col10_text
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

                    text: ""

                    Component.onCompleted: getText()

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

                    function getText() {
                        switch (styleData.column) {
                        case 0:
                            text = table_model.get(styleData.row).col1
                            break
                        case 1:
                            text = table_model.get(styleData.row).col2
                            break
                        case 2:
                            text = table_model.get(styleData.row).col3
                            break
                        case 3:
                            text = table_model.get(styleData.row).col4
                            break
                        case 4:
                            text = table_model.get(styleData.row).col5
                            break
                        case 5:
                            text = table_model.get(styleData.row).col6
                            break
                        case 6:
                            text = table_model.get(styleData.row).col7
                            break
                        case 7:
                            text = table_model.get(styleData.row).col8
                            break
                        case 8:
                            text = table_model.get(styleData.row).col9
                            break
                        case 9:
                            text = table_model.get(styleData.row).col10
                            break
                        default:
                            break
                        }
                    }

                    function writeText() {

                        switch (styleData.column) {
                        case 0:
                            table_model.get(styleData.row).col1 = text
                            break
                        case 1:
                            table_model.get(styleData.row).col2 = text
                            break
                        case 2:
                            table_model.get(styleData.row).col3 = text
                            break
                        case 3:
                            table_model.get(styleData.row).col4 = text
                            break
                        case 4:
                            table_model.get(styleData.row).col5 = text
                            break
                        case 5:
                            table_model.get(styleData.row).col6 = text
                            break
                        case 6:
                            table_model.get(styleData.row).col7 = text
                            break
                        case 7:
                            table_model.get(styleData.row).col8 = text
                            break
                        case 8:
                            table_model.get(styleData.row).col9 = text
                            break
                        case 9:
                            table_model.get(styleData.row).col10 = text
                            break
                        default:
                            break
                        }
                    }
                }
            }
        }
    }
}
