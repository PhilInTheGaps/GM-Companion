import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import "../../../defines.js" as Defines
import FontAwesome 2.0

Column {
    id: root
    spacing: 5

    // Shop Info
    Item {
        id: shop_info_item
        anchors.left: parent.left
        anchors.right: parent.right
        height: Defines.TOOLBAR_HEIGHT

        // Move up and down
        Item {
            id: shop_up_down
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: height / 2

            // Up
            Button {
                id: shop_up
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: parent.height / 2

                background: Rectangle {
                    color: palette.alternateBase
                }

                Text {
                    text: shop_name_field.editMode ? FontAwesome.checkCircle : FontAwesome.chevronUp
                    font.family: FontAwesome.familySolid
                    color: shop_name_field.editMode ? "limegreen" : palette.buttonText
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                onClicked: {
                    if (shop_name_field.editMode) {
                        shop_name_field.editMode = false
                        shop_editor.name = shop_name_field.text
                    } else {
                        shop_editor.moveShop(-1)
                    }
                }
            }

            // Down
            Button {
                id: shop_down_down
                anchors.top: shop_up.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                background: Rectangle {
                    color: palette.alternateBase
                }

                Text {
                    text: shop_name_field.editMode ? FontAwesome.timesCircle : FontAwesome.chevronDown
                    font.family: FontAwesome.familySolid
                    color: shop_name_field.editMode ? "red" : palette.buttonText
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                onClicked: {
                    if (shop_name_field.editMode) {
                        shop_name_field.editMode = false
                        shop_name_field.text = shop_editor.name
                    } else {
                        shop_editor.moveShop(1)
                    }
                }
            }
        }

        // Name of shop
        CustomTextField {
            id: shop_name_field
            text: shop_editor.name
            anchors.left: shop_up_down.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            selectByMouse: true

            property bool editMode: false
            activeFocusOnPress: editMode
            focus: editMode

            Button {
                anchors.top: parent.top
                anchors.right: shop_delete_button.left
                anchors.bottom: parent.bottom
                width: height
                visible: !shop_name_field.editMode

                background: Rectangle {
                    color: "transparent"
                }

                CustomToolBarButton {
                    iconText: FontAwesome.pen
                    onClicked: shop_name_field.editMode = true
                }
            }

            // Delete Shop
            Button {
                id: shop_delete_button
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                width: height

                background: Rectangle {
                    color: palette.button
                }

                Text {
                    text: FontAwesome.trashAlt
                    font.family: FontAwesome.familySolid
                    font.pixelSize: height
                    anchors.fill: parent
                    anchors.margins: 10
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "red"
                }

                onClicked: shop_delete_overlay.visible = true

                // Confirm deletion
                Rectangle {
                    id: shop_delete_overlay
                    visible: false
                    anchors.fill: parent
                    color: palette.alternateBase

                    Text {
                        text: FontAwesome.trashAlt
                        font.family: FontAwesome.familySolid
                        color: palette.buttonText
                        font.pixelSize: parent.height / 3
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        width: height / 2
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    // Yes
                    Button {
                        anchors.top: parent.top
                        anchors.right: parent.right
                        width: parent.width / 2
                        height: parent.height / 2

                        background: Rectangle {
                            color: "transparent"
                        }

                        Text {
                            text: FontAwesome.checkCircle
                            font.family: FontAwesome.familySolid
                            color: "limegreen"
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }

                        onClicked: {
                            shop_editor.deleteShop()
                            shop_delete_overlay.visible = false
                        }
                    }

                    // No
                    Button {
                        anchors.bottom: parent.bottom
                        anchors.right: parent.right
                        width: parent.width / 2
                        height: parent.height / 2

                        background: Rectangle {
                            color: "transparent"
                        }

                        Text {
                            text: FontAwesome.timesCircle
                            font.family: FontAwesome.familySolid
                            color: "red"
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }

                        onClicked: {
                            shop_delete_overlay.visible = false
                        }
                    }
                }
            }
        }
    }

    // Further info
    Row {
        id: shop_properties_row
        width: parent.width
        spacing: 5

        // Owner
        CustomTextField {
            id: shop_owner_textfield
            width: parent.width / 4
            placeholderText: qsTr("Shop Owner")
            selectByMouse: true
            text: shop_editor.owner

            onTextEdited: shop_editor.setOwner(text)
        }

        // Description
        CustomTextField {
            id: shop_description_textfield
            width: parent.width - parent.spacing - shop_owner_textfield.width
            placeholderText: qsTr("Shop Description")
            selectByMouse: true
            text: shop_editor.description

            onTextEdited: shop_editor.setDescription(text)
        }
    }

    Rectangle {
        id: item_header
        anchors.left: parent.left
        anchors.right: parent.right
        height: Defines.TOOLBAR_HEIGHT
        color: palette.button

        Row {
            anchors.fill: parent
            padding: 5
            leftPadding: 10
            rightPadding: 10
            spacing: 10

            Label {
                text: qsTr("Item")
                font.pointSize: 12
                width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 4
                anchors.verticalCenter: parent.verticalCenter
            }

            Label {
                text: qsTr("Price")
                font.pointSize: 12
                width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 5
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignRight
            }

            Label {
                text: qsTr("Category")
                font.pointSize: 12
                width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 5
                anchors.verticalCenter: parent.verticalCenter
            }

            Label {
                text: qsTr("Description")
                font.pointSize: 12
                width: parent.width - parent.rightPadding - x
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    ListView {
        id: shop_items_table
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.height - y

        interactive: true
        clip: true
        focus: true
        keyNavigationEnabled: true
        highlightFollowsCurrentItem: false

        currentIndex: 0
        onModelChanged: currentIndex = 0

        ScrollBar.vertical: CustomScrollBar {
            id: scroll_bar
            visible: shop_items_table.contentHeight > shop_items_table.height
        }

        model: shopEditorItemModel

        delegate: Rectangle {
            id: delegate_item
            height: shop_items_table.currentIndex
                    == index ? (description_text.lineCount
                                > 1 ? description_text.height
                                      + 10 : Defines.TOOLBAR_HEIGHT) : delegate_row.height + 10
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: scroll_bar.visible ? scroll_bar.width : 0
            color: shop_items_table.currentIndex == index ? palette.alternateBase : "transparent"

            Row {
                id: delegate_row
                padding: 5
                leftPadding: 10
                rightPadding: 10
                spacing: 10
                anchors.left: parent.left
                anchors.right: shop_items_table.currentIndex
                               == index ? delete_item_button.left : parent.right
                anchors.verticalCenter: parent.verticalCenter
                height: 30

                Label {
                    text: modelData.name
                    width: (delegate_item.width - parent.leftPadding * 2 - parent.spacing * 2) / 4
                    clip: true
                    elide: Text.ElideRight
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: 12
                }

                Label {
                    text: modelData.price
                    width: (delegate_item.width - parent.leftPadding * 2 - parent.spacing * 2) / 5
                    clip: true
                    elide: Text.ElideRight
                    horizontalAlignment: Text.AlignRight
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: 12
                }

                Label {
                    text: modelData.category
                    width: (delegate_item.width - parent.leftPadding * 2 - parent.spacing * 2) / 5
                    clip: true
                    elide: Text.ElideRight
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: 12
                }

                Label {
                    id: description_text
                    text: modelData.description
                    width: parent.width - x - parent.rightPadding
                    clip: true
                    elide: Text.ElideRight
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: 12
                    wrapMode: shop_items_table.currentIndex == index ? Text.WordWrap : Text.NoWrap
                }
            }

            MouseArea {
                id: mouse_area
                anchors.fill: parent
                onClicked: {
                    shop_items_table.forceActiveFocus()
                    shop_items_table.currentIndex = index
                }
            }

            CustomButton {
                id: delete_item_button
                iconText: FontAwesome.times
                iconItem.font.pointSize: 14
                background: Item {}

                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                visible: shop_items_table.currentIndex == index
                onClicked: shop_editor.deleteItem(index)
            }
        }
    }
}
