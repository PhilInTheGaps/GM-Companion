pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import src
import common
import "../../.."

Column {
    id: root
    spacing: 5

    property var currentShop: ShopTool.editor && ShopTool.editor.currentProject && ShopTool.editor.currentProject.currentCategory ? ShopTool.editor.currentProject.currentCategory.currentShop : undefined

    // Shop Info
    Item {
        id: shop_info_item
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5
        height: Sizes.toolbarHeight

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
                    text: shop_name_field.editMode ? FontAwesome.circleCheck : FontAwesome.chevronUp
                    font.family: FontAwesome.fontSolid.family
                    font.styleName: FontAwesome.fontSolid.styleName
                    color: shop_name_field.editMode ? SettingsManager.colors.success : palette.buttonText
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                onClicked: {
                    if (shop_name_field.editMode) {
                        shop_name_field.editMode = false;
                        root.currentShop.name = shop_name_field.text;
                    } else {
                        ShopTool.editor.moveShop(-1);
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
                    text: shop_name_field.editMode ? FontAwesome.circleXmark : FontAwesome.chevronDown
                    font.family: FontAwesome.fontSolid.family
                    font.styleName: FontAwesome.fontSolid.styleName
                    color: shop_name_field.editMode ? SettingsManager.colors.error : palette.buttonText
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                onClicked: {
                    if (shop_name_field.editMode) {
                        shop_name_field.editMode = false;
                        shop_name_field.text = root.currentShop.name;
                    } else {
                        ShopTool.editor.moveShop(1);
                    }
                }
            }
        }

        // Name of shop
        TextField {
            id: shop_name_field
            text: root.currentShop ? root.currentShop.name : ""
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
                    color: palette.alternateBase
                }

                Text {
                    text: FontAwesome.trash
                    font.family: FontAwesome.fontSolid.family
                    font.styleName: FontAwesome.fontSolid.styleName
                    font.pixelSize: height
                    anchors.fill: parent
                    anchors.margins: 10
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: SettingsManager.colors.error
                }

                onClicked: shop_delete_overlay.visible = true

                // Confirm deletion
                Rectangle {
                    id: shop_delete_overlay
                    visible: false
                    anchors.fill: parent
                    color: palette.alternateBase

                    Text {
                        text: FontAwesome.trash
                        font.family: FontAwesome.fontSolid.family
                        font.styleName: FontAwesome.fontSolid.styleName
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
                            text: FontAwesome.circleCheck
                            font.family: FontAwesome.fontSolid.family
                            font.styleName: FontAwesome.fontSolid.styleName
                            color: SettingsManager.colors.success
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }

                        onClicked: {
                            ShopTool.editor.deleteShop();
                            shop_delete_overlay.visible = false;
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
                            text: FontAwesome.circleXmark
                            font.family: FontAwesome.fontSolid.family
                            font.styleName: FontAwesome.fontSolid.styleName
                            color: SettingsManager.colors.error
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }

                        onClicked: {
                            shop_delete_overlay.visible = false;
                        }
                    }
                }
            }
        }
    }

    // Further info
    Item {
        id: shop_properties_row
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5
        height: Sizes.toolbarHeight

        // Owner
        TextField {
            id: shop_owner_textfield
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: parent.width / 4

            placeholderText: qsTr("Shop Owner")
            selectByMouse: true
            text: root.currentShop ? root.currentShop.owner : ""

            onTextEdited: {
                if (!root.currentShop)
                    return;
                root.currentShop.owner = text;
            }
        }

        // Description
        TextField {
            id: shop_description_textfield
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.left: shop_owner_textfield.right
            anchors.leftMargin: 5

            placeholderText: qsTr("Shop Description")
            selectByMouse: true
            text: root.currentShop ? root.currentShop.description : ""

            onTextEdited: {
                if (!root.currentShop)
                    return;
                root.currentShop.description = text;
            }
        }
    }

    Rectangle {
        id: item_header
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5

        height: Sizes.toolbarHeight
        color: palette.alternateBase

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
        anchors.margins: 5

        height: parent.height - y

        interactive: true
        clip: true
        focus: true
        keyNavigationEnabled: true
        highlightFollowsCurrentItem: false

        currentIndex: 0
        onModelChanged: currentIndex = 0

        ScrollBar.vertical: ScrollBar {
            id: scroll_bar
            visible: shop_items_table.contentHeight > shop_items_table.height
        }

        model: ShopTool.editor ? ShopTool.editor.itemModelShop : []

        delegate: Rectangle {
            id: delegate_item

            required property string name
            required property string price
            required property string category
            required property string description
            required property int index

            height: Sizes.toolbarHeight
            anchors.left: parent ? parent.left : undefined
            anchors.right: parent ? parent.right : undefined
            anchors.rightMargin: scroll_bar.visible ? scroll_bar.width : 0

            color: "transparent"
            border.color: ListView.isCurrentItem ? palette.button : palette.dark
            border.width: ListView.isCurrentItem || mouse_area.containsMouse ? 1 : 0

            ToolTip.text: name + ", " + price + ", " + category + "\n" + description
            ToolTip.delay: 1000
            ToolTip.visible: mouse_area.containsMouse

            Row {
                id: delegate_row
                padding: 5
                leftPadding: 10
                rightPadding: 10
                spacing: 10
                anchors.left: parent.left
                anchors.right: shop_items_table.currentIndex === delegate_item.index ? delete_item_button.left : parent.right
                anchors.verticalCenter: parent.verticalCenter
                height: 30

                Label {
                    text: delegate_item.name
                    width: (delegate_item.width - parent.leftPadding * 2 - parent.spacing * 2) / 4
                    clip: true
                    elide: Text.ElideRight
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: 12

                    ToolTip.text: delegate_item.name
                    ToolTip.delay: 500
                }

                Label {
                    text: delegate_item.price
                    width: (delegate_item.width - parent.leftPadding * 2 - parent.spacing * 2) / 5
                    clip: true
                    elide: Text.ElideRight
                    horizontalAlignment: Text.AlignRight
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: 12

                    ToolTip.text: delegate_item.price
                    ToolTip.delay: 500
                }

                Label {
                    text: delegate_item.category
                    width: (delegate_item.width - parent.leftPadding * 2 - parent.spacing * 2) / 5
                    clip: true
                    elide: Text.ElideRight
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: 12

                    ToolTip.text: delegate_item.category
                    ToolTip.delay: 500
                }

                Label {
                    id: description_text
                    text: delegate_item.description
                    width: parent.width - x - parent.rightPadding
                    clip: true
                    elide: Text.ElideRight
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: 12
                }
            }

            MouseArea {
                id: mouse_area
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    shop_items_table.forceActiveFocus();
                    shop_items_table.currentIndex = delegate_item.index;
                }
            }

            CustomButton {
                id: delete_item_button
                iconText: FontAwesome.xmark
                iconItem.font.pointSize: 14
                background: Item {
                }

                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                visible: shop_items_table.currentIndex === delegate_item.index
                onClicked: ShopTool.editor.deleteItem(delegate_item.index)
            }
        }
    }
}
