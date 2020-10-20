import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../../../defines.js" as Defines

Page {
    id: root

    header: Rectangle {
        height: Defines.TOOLBAR_HEIGHT
        color: palette.button

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

        model: shopEditorItemModel2

        delegate: Rectangle {
            id: item_delegate_rect
            height: item_list_table.currentIndex
                    == index ? Math.max(
                                   item_delegate_text_column.height,
                                   Defines.TOOLBAR_HEIGHT) : item_delegate_text_column.height
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: scroll_bar.visible ? scroll_bar.width : 0
            color: item_list_table.currentIndex == index ? palette.button : "transparent"

            Column {
                id: item_delegate_text_column
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: item_list_table.currentIndex
                              == index ? add_item_button.right : parent.left
                anchors.right: parent.right
                anchors.margins: 5

                Item {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: Math.max(30, item_name_text.height,
                                     item_price_text.height)

                    Label {
                        id: item_name_text
                        anchors.left: parent.left
                        anchors.right: item_price_text.left
                        anchors.verticalCenter: parent.verticalCenter
                        verticalAlignment: Text.AlignVCenter

                        text: modelData.name
                        font.pointSize: 12

                        clip: true
                        elide: Text.ElideRight
                        wrapMode: item_list_table.currentIndex == index ? Text.Wrap : Text.NoWrap
                    }

                    Label {
                        id: item_price_text
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        width: item_list_table.priceLabelWidth
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight

                        text: modelData.price
                        font.pointSize: 12

                        clip: true
                        elide: Text.ElideRight
                        wrapMode: item_list_table.currentIndex
                                  == index ? Text.WordWrap : Text.NoWrap
                    }
                }

                Label {
                    id: item_description_text
                    anchors.left: parent.left
                    anchors.right: parent.right
                    verticalAlignment: Text.AlignTop
                    visible: item_list_table.currentIndex == index
                             && modelData.description !== ""

                    text: modelData.description
                    font.pointSize: 12

                    clip: true
                    elide: Text.ElideRight
                    wrapMode: item_list_table.currentIndex == index ? Text.WordWrap : Text.NoWrap
                }

                Rectangle {
                    visible: item_description_text.visible
                    height: 5
                    width: 5
                    color: "transparent"
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    item_list_table.forceActiveFocus()
                    item_list_table.currentIndex = index
                }
            }

            CustomButton {
                id: add_item_button
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                background: Item {}
                iconItem.font.pointSize: 14

                visible: item_list_table.currentIndex == index

                iconText: FontAwesome.chevronCircleLeft
                onClicked: shop_editor.addItem(index)
            }
        }
    }
}
