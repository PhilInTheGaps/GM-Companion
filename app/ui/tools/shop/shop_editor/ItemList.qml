pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import src
import "../../.."

Page {
    id: root

    header: Rectangle {
        height: Sizes.toolbarHeight
        color: palette.alternateBase

        Label {
            text: qsTr("Item")
            font.pointSize: 12

            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: 10
        }

        Label {
            text: qsTr("Price")
            font.pointSize: 12

            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: 10
        }
    }

    contentItem: ListView {
        id: item_list_table

        readonly property int priceLabelWidth: width / 3

        interactive: true
        clip: true
        focus: true
        keyNavigationEnabled: true
        highlightFollowsCurrentItem: false

        currentIndex: 0
        onModelChanged: currentIndex = 0

        ScrollBar.vertical: ScrollBar {
            id: scroll_bar
            visible: item_list_table.contentHeight > item_list_table.height
        }

        model: ShopTool && ShopTool.editor ? ShopTool.editor.itemModelGroup : [] // qmllint disable unresolved-type

        delegate: Rectangle {
            id: item_delegate_rect

            required property string name
            required property string price
            required property string category
            required property string description
            required property int index

            height: item_delegate_text_column.height
            anchors.left: parent ? parent.left : undefined
            anchors.right: parent ? parent.right : undefined
            anchors.rightMargin: scroll_bar.visible ? scroll_bar.width : 0

            color: "transparent"
            border.color: ListView.isCurrentItem ? palette.button : palette.dark
            border.width: ListView.isCurrentItem || mouse_area.containsMouse ? 1 : 0

            ToolTip.text: name + ", " + price + ", " + category + "\n" + description
            ToolTip.delay: 1000
            ToolTip.visible: mouse_area.containsMouse

            Column {
                id: item_delegate_text_column
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: item_list_table.currentIndex === item_delegate_rect.index ? add_item_button.right : parent.left
                anchors.right: parent.right
                anchors.margins: 5

                Item {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: Math.max(30, item_name_text.height, item_price_text.height)

                    Label {
                        id: item_name_text
                        anchors.left: parent.left
                        anchors.right: item_price_text.left
                        anchors.verticalCenter: parent.verticalCenter
                        verticalAlignment: Text.AlignVCenter

                        text: item_delegate_rect.name
                        font.pointSize: 12

                        clip: true
                        elide: Text.ElideRight
                        wrapMode: item_list_table.currentIndex === item_delegate_rect.index ? Text.Wrap : Text.NoWrap
                    }

                    Label {
                        id: item_price_text
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        width: item_list_table.priceLabelWidth
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight

                        text: item_delegate_rect.price
                        font.pointSize: 12

                        clip: true
                        elide: Text.ElideRight
                        wrapMode: item_list_table.currentIndex === item_delegate_rect.index ? Text.WordWrap : Text.NoWrap
                    }
                }

                Label {
                    id: item_description_text
                    anchors.left: parent.left
                    anchors.right: parent.right
                    verticalAlignment: Text.AlignTop
                    visible: item_list_table.currentIndex === item_delegate_rect.index && item_delegate_rect.description !== ""

                    text: item_delegate_rect.description
                    font.pointSize: 12

                    clip: true
                    elide: Text.ElideRight
                    wrapMode: item_list_table.currentIndex === item_delegate_rect.index ? Text.WordWrap : Text.NoWrap
                }

                Rectangle {
                    visible: item_description_text.visible
                    height: 5
                    width: 5
                    color: "transparent"
                }
            }

            MouseArea {
                id: mouse_area
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    item_list_table.forceActiveFocus();
                    item_list_table.currentIndex = item_delegate_rect.index;
                }
            }

            CustomButton {
                id: add_item_button
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                background: Item {
                }
                iconItem.font.pointSize: 14

                visible: item_list_table.currentIndex === item_delegate_rect.index

                iconText: FontAwesome.circleChevronLeft
                onClicked: ShopTool.editor.addItem(item_delegate_rect.index)
            }
        }
    }
}
