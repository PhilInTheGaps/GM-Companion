import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4 as Controls1_4

import gm.companion.shoptool 1.0
import gm.companion.shopeditor 1.0
import "../../fontawesome"

Page {
    id: editor

    signal backToViewer
    signal switchToItemEditor

    ShopTool {
        id: shop_tool
    }

    ShopEditorTool {
        id: editor_tool

        function updateShopFlow() {
            updateShopList()

            shop_column.children = []

            var component = Qt.createComponent("./ShopButton.qml")

            console.log(getShopList())

            for (var i = 0; i < getShopList().length; i++) {

                console.log(getShopList()[i])

                var button = component.createObject(shop_column, {
                                                        x: 0,
                                                        y: 0,
                                                        shop: getShopList()[i]
                                                    })

                button.clicked.connect(updateShopInformation)
            }
        }

        function updateShopInformation(shop) {
            loadShop(shop)

            shop_name_text.text = getShopName()
            shop_owner_textfield.text = getShopOwner()
            shop_description_textfield.text = getShopDescription()

            fillItemTable()
        }

        function fillItemTable() {
            table_model.clear()

            for (var i = 0; i < getItemNames().length; i++) {
                table_model.append({
                                       name: getItemNames()[i],
                                       price: getItemPrices()[i],
                                       category: getItemCategories()[i],
                                       description: getItemDescriptions()[i]
                                   })
            }
        }

        function fillItemListTable() {
            item_list_model.clear()

            for (var i = 0; i < getItemListNames().length; i++) {
                item_list_model.append({
                                           name: getItemListNames()[i],
                                           price: getItemListPrices()[i],
                                           category: getItemListCategories()[i],
                                           description: getItemListDescriptions(
                                                            )[i]
                                       })
            }
        }

        function addListItem(index) {
            addItem(getListItem(index))

            fillItemTable()
        }
    }

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    header: tool_bar

    Rectangle {
        id: tool_bar
        color: color_scheme.toolbarColor
        height: color_scheme.toolbarHeight

        Row {
            anchors.fill: parent
            spacing: 5

            Button {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                hoverEnabled: true

                background: Rectangle {
                    color: "transparent"
                }

                Icon {
                    icon: icons.far_arrow_alt_circle_left
                    pointSize: 25
                    anchors.centerIn: parent
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                }

                width: height
                onClicked: backToViewer()
            }

            Button {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: new_project_text.width + 10
                hoverEnabled: true

                Text {
                    id: new_project_text
                    text: qsTr("Create New Project")
                    anchors.centerIn: parent
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                    font.bold: true
                    font.pointSize: 12
                }

                background: Rectangle {
                    color: "transparent"
                }

                onClicked: {
                    new_project_dialog.open()
                }
            }

            Button {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: new_category_text.width + 10
                hoverEnabled: true

                Text {
                    id: new_category_text
                    text: qsTr("Create New Category")
                    anchors.centerIn: parent
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                    font.bold: true
                    font.pointSize: 12
                }

                background: Rectangle {
                    color: "transparent"
                }

                onClicked: {
                    new_category_dialog.open()
                }
            }

            Button {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: new_shop_text.width + 10
                hoverEnabled: true

                Text {
                    id: new_shop_text
                    text: qsTr("Create New Shop")
                    anchors.centerIn: parent
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                    font.bold: true
                    font.pointSize: 12
                }

                background: Rectangle {
                    color: "transparent"
                }

                onClicked: {
                    new_shop_dialog.open()
                }
            }
        }

        Button {
            id: open_item_editor_button
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: open_item_editor_row.width
            hoverEnabled: true
            x: parent.width - width - 5

            background: Rectangle {
                color: "transparent"
            }

            Row {
                id: open_item_editor_row
                leftPadding: 5
                rightPadding: 5
                spacing: 5
                anchors.verticalCenter: parent.verticalCenter
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                x: parent.width - width - 5

                Icon {
                    icon: icons.far_edit
                    pointSize: 12
                    color: parent.parent.pressed ? "grey" : parent.parent.hovered ? "lightgrey" : color_scheme.toolbarTextColor
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    id: open_item_editor_text
                    text: qsTr("Open Item Editor")
                    color: parent.parent.pressed ? "grey" : parent.parent.hovered ? "lightgrey" : color_scheme.toolbarTextColor
                    font.pointSize: 12
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            onClicked: switchToItemEditor()
        }
    }

    Column {
        width: parent.width
        height: parent.height
        spacing: 5
        padding: 5

        Dialog {
            id: new_project_dialog
            title: qsTr("Create New Project")

            contentItem: Column {
                TextField {
                    id: project_textfield
                    width: parent.width
                    placeholderText: qsTr("Project Name")
                    selectByMouse: true
                }
            }

            standardButtons: Dialog.Ok | Dialog.Cancel

            onAccepted: {
                editor_tool.createProject(project_textfield.text)

                project_combo_box.model = shop_tool.projects
            }
        }

        Dialog {
            id: new_category_dialog
            title: qsTr("Create New Category")

            contentItem: Column {

                TextField {
                    id: category_textfield
                    width: parent.width
                    placeholderText: qsTr("Category Name")
                    selectByMouse: true
                }
            }

            standardButtons: Dialog.Ok | Dialog.Cancel

            onAccepted: {
                editor_tool.createCategory(category_textfield.text)

                category_combo_box.model = editor_tool.getCategories()

                category_combo_box.currentIndex = editor_tool.getCategories(
                            ).length - 1
            }
        }

        Dialog {
            id: new_shop_dialog
            title: qsTr("Create New Shop")

            contentItem: Column {

                TextField {
                    id: shop_name_textfield
                    width: parent.width
                    placeholderText: qsTr("Shop Name")
                    selectByMouse: true
                }
            }

            standardButtons: Dialog.Ok | Dialog.Cancel

            onAccepted: {
                editor_tool.createShop(shop_name_textfield.text)

                editor_tool.updateShopFlow()
            }
        }

        Row {
            width: parent.width - parent.padding * 2
            height: parent.height - parent.padding * 2
            spacing: 5

            Column {
                id: shop_properties_column
                width: 175
                height: parent.height
                spacing: 5

                Text {
                    id: project_text
                    text: qsTr("Project")
                    color: color_scheme.textColor
                }

                ComboBox {
                    id: project_combo_box
                    width: parent.width
                    model: shop_tool.projects

                    onCurrentTextChanged: {
                        if (currentText != "") {
                            editor_tool.setCurrentProject(currentText)

                            category_combo_box.model = editor_tool.getCategories()
                        }
                    }
                }

                Text {
                    id: category_text
                    text: qsTr("Category")
                    color: color_scheme.textColor
                }

                ComboBox {
                    id: category_combo_box
                    width: parent.width
                    Component.onCompleted: {
                        model = editor_tool.getCategories()
                    }

                    onCurrentTextChanged: {
                        if (currentText != "") {
                            editor_tool.setCurrentCategory(currentText)

                            editor_tool.updateShopFlow()
                        }
                    }
                }

                Text {
                    id: shops_text
                    text: qsTr("Shops")
                    color: color_scheme.textColor
                }

                ScrollView {
                    width: parent.width
                    height: parent.height - parent.spacing * 5 - category_combo_box.height
                            - category_text.height - shops_text.height
                            - project_combo_box.height - project_text.height

                    clip: true

                    Column {
                        id: shop_column
                        width: parent.parent.width
                        spacing: 5
                    }
                }
            }

            Column {
                width: parent.width - parent.spacing - shop_properties_column.width
                height: parent.height
                spacing: 5

                Row {
                    width: parent.width
                    height: parent.height
                    spacing: 5

                    Column {
                        width: (parent.width - parent.spacing) / 2
                        height: parent.height
                        spacing: 5

                        Rectangle {
                            id: shop_action_row
                            width: parent.width
                            color: color_scheme.toolbarColor
                            height: color_scheme.toolbarHeight

                            Row {
                                anchors.fill: parent
                                spacing: 5
                                leftPadding: 5
                                rightPadding: 5

                                Button {
                                    width: save_shop_row.width
                                    hoverEnabled: true

                                    height: parent.height
                                    background: Rectangle {
                                        color: "transparent"
                                    }

                                    Row {
                                        id: save_shop_row
                                        leftPadding: 5
                                        rightPadding: 5
                                        spacing: 5
                                        anchors.centerIn: parent

                                        Icon {
                                            icon: icons.far_save
                                            pointSize: 12
                                            color: parent.parent.pressed ? "grey" : parent.parent.hovered ? "lightgrey" : color_scheme.toolbarTextColor
                                            anchors.verticalCenter: parent.verticalCenter
                                        }

                                        Text {
                                            text: qsTr("Save Shop")
                                            color: parent.parent.pressed ? "grey" : parent.parent.hovered ? "lightgrey" : color_scheme.toolbarTextColor
                                            font.pointSize: 12
                                            font.bold: true
                                            anchors.verticalCenter: parent.verticalCenter
                                        }
                                    }

                                    onClicked: editor_tool.saveShop()
                                }

                                DelayButton {
                                    width: delete_shop_row.width
                                    delay: 1200
                                    ToolTip.text: qsTr("Hold to delete shop")
                                    ToolTip.visible: hovered
                                    hoverEnabled: true

                                    height: parent.height
                                    background: Rectangle {
                                        color: "transparent"
                                    }

                                    Row {
                                        id: delete_shop_row
                                        leftPadding: 5
                                        rightPadding: 5
                                        spacing: 5
                                        anchors.centerIn: parent

                                        Icon {
                                            icon: icons.far_trash_alt
                                            pointSize: 12
                                            color: parent.parent.pressed ? "grey" : parent.parent.hovered ? "lightgrey" : color_scheme.toolbarTextColor
                                            anchors.verticalCenter: parent.verticalCenter
                                        }

                                        Text {
                                            text: qsTr("Delete Shop")
                                            color: parent.parent.pressed ? "grey" : parent.parent.hovered ? "lightgrey" : color_scheme.toolbarTextColor
                                            font.pointSize: 12
                                            font.bold: true
                                            anchors.verticalCenter: parent.verticalCenter
                                        }
                                    }

                                    onActivated: {
                                        editor_tool.deleteShop()

                                        editor_tool.updateShopFlow()
                                    }
                                }

                                DelayButton {
                                    width: remove_all_items_row.width
                                    delay: 1200

                                    ToolTip.text: qsTr("Hold to remove all items")
                                    ToolTip.visible: hovered
                                    hoverEnabled: true

                                    height: parent.height
                                    background: Rectangle {
                                        color: "transparent"
                                    }

                                    Row {
                                        id: remove_all_items_row
                                        leftPadding: 5
                                        rightPadding: 5
                                        spacing: 5
                                        anchors.centerIn: parent

                                        Icon {
                                            icon: icons.fas_times
                                            pointSize: 12
                                            color: parent.parent.pressed ? "grey" : parent.parent.hovered ? "lightgrey" : color_scheme.toolbarTextColor
                                            anchors.verticalCenter: parent.verticalCenter
                                        }

                                        Text {
                                            text: qsTr("Remove All Items")
                                            color: parent.parent.pressed ? "grey" : parent.parent.hovered ? "lightgrey" : color_scheme.toolbarTextColor
                                            font.pointSize: 12
                                            font.bold: true
                                            anchors.verticalCenter: parent.verticalCenter
                                        }
                                    }

                                    onActivated: {
                                        editor_tool.removeAllItems()
                                        editor_tool.fillItemTable()
                                    }
                                }
                            }
                        }

                        Text {
                            id: shop_name_text
                            text: qsTr("No Shop Selected")
                            font.pointSize: 20
                            color: color_scheme.textColor
                            verticalAlignment: Text.AlignVCenter

                            width: parent.width
                            height: item_tab_bar.height
                            clip: true
                            elide: Text.ElideRight
                        }

                        Row {
                            id: shop_properties_row
                            width: parent.width
                            spacing: 5

                            TextField {
                                id: shop_owner_textfield
                                width: parent.width / 4
                                placeholderText: qsTr("Shop Owner")
                                selectByMouse: true

                                onTextEdited: editor_tool.setShopOwner(text)
                            }

                            TextField {
                                id: shop_description_textfield
                                width: parent.width - parent.spacing - shop_owner_textfield.width
                                placeholderText: qsTr("Shop Description")
                                selectByMouse: true

                                onTextEdited: editor_tool.setShopDescription(
                                                  text)
                            }
                        }

                        Rectangle {
                            id: item_header
                            anchors.left: parent.left
                            anchors.right: parent.right
                            height: color_scheme.toolbarHeight
                            color: color_scheme.listHeaderBackgroundColor

                            Row {
                                anchors.fill: parent
                                padding: 5
                                leftPadding: 10
                                rightPadding: 10
                                spacing: 10

                                Text {
                                    text: qsTr("Item")
                                    color: color_scheme.listHeaderTextColor
                                    font.pointSize: 12
                                    font.bold: true
                                    width: (parent.width - parent.leftPadding
                                            * 2 - parent.spacing * 2) / 4
                                    anchors.verticalCenter: parent.verticalCenter
                                }

                                Text {
                                    text: qsTr("Price")
                                    color: color_scheme.listHeaderTextColor
                                    font.pointSize: 12
                                    font.bold: true
                                    width: (parent.width - parent.leftPadding
                                            * 2 - parent.spacing * 2) / 5
                                    anchors.verticalCenter: parent.verticalCenter
                                }

                                Text {
                                    text: qsTr("Category")
                                    color: color_scheme.listHeaderTextColor
                                    font.pointSize: 12
                                    font.bold: true
                                    width: (parent.width - parent.leftPadding
                                            * 2 - parent.spacing * 2) / 5
                                    anchors.verticalCenter: parent.verticalCenter
                                }

                                Text {
                                    text: qsTr("Description")
                                    color: color_scheme.listHeaderTextColor
                                    font.pointSize: 12
                                    font.bold: true
                                    width: parent.width - parent.rightPadding - x
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }
                        }

                        ListView {
                            id: shop_items_table
                            anchors.left: parent.left
                            anchors.right: parent.right

                            height: parent.height - parent.spacing * 4
                                    - shop_properties_row.height - shop_name_text.height
                                    - shop_action_row.height - item_header.height

                            clip: true
                            spacing: 0

                            ScrollBar.vertical: ScrollBar {
                            }

                            model: ListModel {
                                id: table_model
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
                                    height: 30

                                    Text {
                                        text: name
                                        color: mouse_area.pressed ? "grey" : color_scheme.textColor
                                        width: (parent.width - parent.leftPadding
                                                * 2 - parent.spacing * 2) / 4
                                        clip: true
                                        elide: Text.ElideRight
                                        anchors.verticalCenter: parent.verticalCenter
                                        font.pointSize: 12
                                    }

                                    Text {
                                        text: price
                                        color: color_scheme.textColor
                                        width: (parent.width - parent.leftPadding
                                                * 2 - parent.spacing * 2) / 5
                                        clip: true
                                        elide: Text.ElideRight
                                        anchors.verticalCenter: parent.verticalCenter
                                        font.pointSize: 12
                                    }

                                    Text {
                                        text: category
                                        color: color_scheme.textColor
                                        width: (parent.width - parent.leftPadding
                                                * 2 - parent.spacing * 2) / 5
                                        clip: true
                                        elide: Text.ElideRight
                                        anchors.verticalCenter: parent.verticalCenter
                                        font.pointSize: 12
                                    }

                                    Text {
                                        text: description
                                        color: color_scheme.textColor
                                        width: (parent.width - x - parent.rightPadding)
                                               - x_button.width - parent.spacing
                                        clip: true
                                        elide: Text.ElideRight
                                        anchors.verticalCenter: parent.verticalCenter
                                        font.pointSize: 12
                                    }

                                    Button {
                                        id: x_button
                                        height: parent.height - parent.topPadding * 2
                                        width: height
                                        visible: mouse_area.containsMouse

                                        background: Rectangle {
                                            color: "transparent"
                                        }

                                        Icon {
                                            icon: icons.fas_times
                                            pointSize: 12
                                            anchors.centerIn: parent
                                            color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : color_scheme.toolbarTextColor
                                        }

                                        onClicked: {
                                            editor_tool.removeItem(index)
                                            table_model.remove(index)
                                        }
                                    }
                                }
                            }
                        }
                    }

                    Column {
                        width: (parent.width - parent.spacing) / 2
                        height: parent.height
                        spacing: 5

                        TabBar {
                            id: item_tab_bar
                            width: parent.width
                            height: color_scheme.toolbarHeight

                            background: Rectangle {
                                color: color_scheme.toolbarColor
                            }

                            spacing: 5

                            onCurrentIndexChanged: {
                                console.log(currentIndex)

                                editor_tool.setItemListTabIndex(currentIndex)
                                editor_tool.loadItemList()
                                editor_tool.fillItemListTable()
                            }

                            Repeater {
                                id: item_tab_bar_repeater
                                model: 0

                                TabButton {
                                    hoverEnabled: true
                                    height: parent.height
                                    anchors.verticalCenter: parent.verticalCenter

                                    Text {
                                        text: editor_tool.getItemListTabNames(
                                                  )[index]
                                        color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : color_scheme.toolbarTextColor
                                        font.pointSize: 12
                                        font.bold: true
                                        anchors.centerIn: parent
                                    }

                                    background: Rectangle {
                                        color: "transparent"
                                    }
                                }
                            }
                        }

                        Rectangle {
                            id: item_list_header
                            anchors.left: parent.left
                            anchors.right: parent.right
                            height: color_scheme.toolbarHeight
                            color: color_scheme.listHeaderBackgroundColor

                            Row {
                                anchors.fill: parent
                                padding: 5
                                leftPadding: 10
                                rightPadding: 10
                                spacing: 10

                                Text {
                                    text: qsTr("Item")
                                    color: color_scheme.listHeaderTextColor
                                    font.pointSize: 12
                                    font.bold: true
                                    width: (parent.width - parent.leftPadding
                                            * 2 - parent.spacing * 2) / 4
                                    anchors.verticalCenter: parent.verticalCenter
                                }

                                Text {
                                    text: qsTr("Price")
                                    color: color_scheme.listHeaderTextColor
                                    font.pointSize: 12
                                    font.bold: true
                                    width: (parent.width - parent.leftPadding
                                            * 2 - parent.spacing * 2) / 5
                                    anchors.verticalCenter: parent.verticalCenter
                                }

                                Text {
                                    text: qsTr("Category")
                                    color: color_scheme.listHeaderTextColor
                                    font.pointSize: 12
                                    font.bold: true
                                    width: (parent.width - parent.leftPadding
                                            * 2 - parent.spacing * 2) / 5
                                    anchors.verticalCenter: parent.verticalCenter
                                }

                                Text {
                                    text: qsTr("Description")
                                    color: color_scheme.listHeaderTextColor
                                    font.pointSize: 12
                                    font.bold: true
                                    width: parent.width - parent.rightPadding - x
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }
                        }

                        ListView {
                            id: item_list_table
                            anchors.left: parent.left
                            anchors.right: parent.right

                            height: parent.height - parent.spacing * 2
                                    - item_tab_bar.height - item_list_header.height

                            clip: true
                            spacing: 0

                            ScrollBar.vertical: ScrollBar {
                            }

                            model: ListModel {
                                id: item_list_model
                            }

                            Component.onCompleted: {
                                editor_tool.loadItemListTabs()
                                item_tab_bar_repeater.model = editor_tool.getItemListTabNames()

                                editor_tool.loadItemList()
                                editor_tool.fillItemListTable()
                            }

                            delegate: Rectangle {
                                height: delegate_row2.height
                                anchors.left: parent.left
                                anchors.right: parent.right
                                color: "transparent"

                                MouseArea {
                                    id: mouse_area2
                                    anchors.fill: parent
                                    hoverEnabled: true

                                    z: 2

                                    onClicked: {
                                        editor_tool.addListItem(index)
                                    }
                                }

                                Row {
                                    id: delegate_row2
                                    padding: 5
                                    leftPadding: 10
                                    rightPadding: 10
                                    spacing: 10
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    height: 30

                                    Icon {
                                        id: add_icon
                                        icon: icons.fas_plus
                                        color: mouse_area2.pressed ? "grey" : "darkgreen"
                                        pointSize: 15
                                        anchors.verticalCenter: parent.verticalCenter
                                        visible: mouse_area2.containsMouse
                                    }

                                    Text {
                                        text: name
                                        color: mouse_area2.pressed ? "grey" : color_scheme.textColor
                                        width: (parent.width - parent.leftPadding
                                                * 2 - parent.spacing * 2) / 4
                                               - (add_icon.visible ? add_icon.width
                                                                     + parent.spacing : 0)
                                        clip: true
                                        elide: Text.ElideRight
                                        anchors.verticalCenter: parent.verticalCenter
                                        font.pointSize: 12
                                    }

                                    Text {
                                        text: price
                                        color: color_scheme.textColor
                                        width: (parent.width - parent.leftPadding
                                                * 2 - parent.spacing * 2) / 5
                                        clip: true
                                        elide: Text.ElideRight
                                        anchors.verticalCenter: parent.verticalCenter
                                        font.pointSize: 12
                                    }

                                    Text {
                                        text: category
                                        color: color_scheme.textColor
                                        width: (parent.width - parent.leftPadding
                                                * 2 - parent.spacing * 2) / 5
                                        clip: true
                                        elide: Text.ElideRight
                                        anchors.verticalCenter: parent.verticalCenter
                                        font.pointSize: 12
                                    }

                                    Text {
                                        text: description
                                        color: color_scheme.textColor
                                        width: (parent.width - x - parent.rightPadding)
                                        clip: true
                                        elide: Text.ElideRight
                                        anchors.verticalCenter: parent.verticalCenter
                                        font.pointSize: 12
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
